#!/bin/sh

./dap proxy rweb --delete
./dap proxy rweba --delete
./dap proxy rweb6 --delete

if [ "$(config get wan_proto)" = "pppoe" -o "$(config get wan_proto)" = "l2tp" -o "$(config get wan_proto)" = "pptp" ]; then
	wan_inet_addr=`ifconfig ppp0 | grep "inet addr" | awk '{print $2}' |awk -F ':' '{print $2}'`
else
	wan_inet_addr=`ifconfig brwan | grep "inet addr" | awk '{print $2}' |awk -F ':' '{print $2}'`
fi

if [ "x$wan_inet_addr" != "x" ]; then
	if [ "$(config get remote_endis)" = "1" ]; then
		./dap proxy rweb https://$wan_inet_addr:443 https://127.0.0.1:8082
	fi
fi

apmode_inet_addr=`ifconfig br0 | grep "inet addr" | awk '{print $2}' | awk -F ':' '{print $2}'`
if [ "x$apmode_inet_addr" != "x" ]; then
	if [ "$(config get ap_mode)" = "1" ]; then
		if [ "$(config get remote_endis)" = "1" ]; then
			./dap proxy rweba https://$apmode_inet_addr:443 https://127.0.0.1:8082
		fi
	fi
fi

wan_inet6_addr=`ifconfig brwan | grep Global | awk '{print $3}' | awk -F '/' '{print $1}'`
if [ "x$wan_inet6_addr" != "x" ]; then
	if [ "$(config get remote_endis)" = "1" ]; then
		./dap proxy rweb6 https://[$wan_inet6_addr]:443 https://127.0.0.1:8082
	fi
fi
