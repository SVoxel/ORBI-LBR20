#!/bin/sh

config_dap(){
	cd /media/nand
	cp /etc/dap.zip .
	unzip -o dap.zip
	if [ $? = "0" ]
	then
		cp -f /media/nand/dap_data/dap /media/nand/dap
	else
		echo "Failed to unzip dap.zip package!"
		exit
	fi

}

if [ ! -f /media/nand/dap.zip ]
then
	echo "No DAP package in flash!"
	config_dap
else
	fw_dap=`md5sum /etc/dap.zip 2>/dev/null | awk '{print $1}'` 
	local_dap=`md5sum /media/nand/dap.zip 2>/dev/null | awk '{print $1}'`
	if [ "x$fw_dap" != "x$local_dap" ]
	then
		echo "Different version of DAP package detected in flash!"
		config_dap
	fi
fi

/etc/verify_dap
if [ $? = "0" ];then
	echo "The SDK is intact"
else if [ $? = "1" ];then
	echo "The SDK has been tampered with"
	config_dap
fi
fi

while true
do
	echo "Waiting for httpd up!"
	ps | grep httpd | grep -v grep > /dev/null
	if [ $? = "0" ]
	then
		echo "httpd started!Start dap now!"
		cd /media/nand
		./dap_port_init.sh
		if [ -f ./dap_port_reinit.sh ]
		then
			./dap_port_reinit.sh
		fi
		./dap service restart
		echo "check_dap.sh done!"
		break
	else
		echo "wait 1 sec."
		sleep 1
	fi
done

if [ "x$(config get forceshield_reset_flag)" = "x1" ]; then
	./dap reset
	./dap proxy web http://$(config get lan_ipaddr):80 http://127.0.0.1:8080
	./dap proxy webs https://$(config get lan_ipaddr):443 https://127.0.0.1:8082
	./dap service restart
	./dap attackcount -c
	config set forceshield_reset_flag=0
fi
