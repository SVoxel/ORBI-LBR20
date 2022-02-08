#!/bin/sh

SERVER_URL="http.fw.updates1.netgear.com"
FORCESHIELD_VERSION_INFO=/tmp/forceshield_version_info
UPDATE_PATH=/tmp/forceshield_update_path
CURRENT_VERSION=/tmp/forceshield_current_version
DAP_DIR=/media/nand

curl --insecure https://$SERVER_URL/sw-apps/bot-protection/rbr50/fileinfo.txt | grep version | awk -F '=' '{printf $2}' > $FORCESHIELD_VERSION_INFO
curl --insecure https://$SERVER_URL/sw-apps/bot-protection/rbr50/fileinfo.txt | grep file | awk -F '=' '{printf $2}' > $UPDATE_PATH

update_path=`cat $UPDATE_PATH`
/media/nand/dap version > $CURRENT_VERSION
if [ "x`cat $CURRENT_VERSION`" = "x`cat $FORCESHIELD_VERSION_INFO`" ]; then
	echo "The forceshield dap on update server is the same as on R/W filesystem"
else
	[ -d $DAP_DIR ] && cd $DAP_DIR
	./dap upgrade $update_path
	./dap service restart
	#/etc/init.d/uhttpd stop
	#/etc/init.d/uhttpd start
fi
