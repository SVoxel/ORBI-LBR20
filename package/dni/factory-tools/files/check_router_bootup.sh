#!/bin/sh

LTE_READY=/tmp/lte_bootup_successful
WIFI_READY=/tmp/wifi_bootup_successful
ROUTER_READY=/tmp/router_bootup_successful
try_cnt=0
while [ 1 ] 
do
	if [ -f $LTE_READY ] && [ -f $WIFI_READY ];then
		echo "ready" > $ROUTER_READY
		echo "[factory test] lte and wifi is up ready" > /dev/console
		/sbin/ledcontrol -n power -c amber -s on
		exit 0
	else
		sleep 2
		let try_cnt+=1
	fi
	[ $try_cnt -gt 150 ]&& exit 1
done
