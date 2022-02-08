#!/bin/sh

./dap proxy web --delete
./dap proxy webs --delete
./dap proxy web6 --delete
inet_addr=`ifconfig br0 | grep "inet addr" | awk '{print $2}' | awk -F ':' '{print $2}'`
if [ "x$inet_addr" != "x" ]; then
	./dap proxy web http://$inet_addr:80 http://127.0.0.1:8080
	./dap proxy webs https://$inet_addr:443 https://127.0.0.1:8082
fi

inet6_addr=`ifconfig br0 | grep Global | awk '{print $3}' | awk -F '/' '{print $1}'`
if [ "x$inet6_addr" != "x" ]; then
	./dap proxy web6 http://[$inet6_addr]:80 http://127.0.0.1:8080
fi

while true
do
	dap_addr=`./dap show | grep 8080 | awk -F '//' '{print $2}' | awk -F ':' '{print $1}'`
	inet_addr=`ifconfig br0 | grep "inet addr" | awk '{print $2}' | awk -F ':' '{print $2}'`
	if [ "x$inet_addr" != "x$dap_addr" ];then
	  	if [ "x$inet_addr" != "x" ]; then
	    		./dap proxy web http://$inet_addr:80 http://127.0.0.1:8080
	      		./dap proxy webs https://$inet_addr:443 https://127.0.0.1:8082
	       	fi
		sleep 1
	else
		break
	fi
done

