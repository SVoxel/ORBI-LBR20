#!/bin/bash

AUTO_CMD=$(which expect)
GIT_BIN=$(which git)
local_password='123456'

#call expect to execute git-pull automatically without input password manually.
function expect_fun()
{
	eval ${AUTO_CMD} <<- EOF
		set timeout -1
		set password $local_password
		spawn $GIT_BIN pull
		expect {
			"*password: " { send "$local_password\r" }
		}
		expect eof
		EOF
}
prepare()
{
	[ -f dni_home/Maintrunk_Commit_ID ] && tag=`cat dni_home/Maintrunk_Commit_ID |awk -F":" '{print $2}'`
	[ "x$tag" != "x" ] && git reset --hard $tag && sleep 2
	cd dni_home
	git checkout package/
	git checkout kernel/
	cd -
}
collect_fail_module()
{
	module_name=`tail -2 Compile-log.txt | head -1 |grep compile | awk  -F "-C " '{print $2}' | awk '{print $1}' |awk -F "/" '{print $NF}'`
	[ "x$module_name" = "x" ] && echo "unknow error, exit" && exit
	echo $module_name >> Compile-error-module.txt
	if [ -d dni_home/package/$module_name ];then
		if [ "x`ls dni_home/package/$module_name/patches/ |head -1`" = "x" ];then
			[ -f package/dni/$module_name/Makefile ] && sed -i "/eval/d" package/dni/$module_name/Makefile
		else
			[ "x`ls dni_home/package/$module_name/patches/ |head -1`" = "x" ] && echo "project local patches is none, exit" && exit
			rm -rf dni_home/package/$module_name/patches/*
		fi
	elif [ "x$module_name" = "xlinux" ];then 
		[ "x`ls dni_home/kernel/patches_dni/ |head -1`" = "x" ] && echo "project kernel local patches is none, exit" && exit
		rm -rf dni_home/kernel/patches_dni/*
		rm -rf build_dir/target-arm_cortex-a7_uClibc-1.0.14_eabi/linux-ipq806x/
	else
		[ -f package/dni/$module_name/Makefile ] && sed -i "/eval/d" package/dni/$module_name/Makefile
	fi
}

function compile_fun()
{
	[ "x$1" = "x" -o "x$2" = "x" -o "x$3" = "x" ] && echo "Miss some parameter..." && return
	echo "Start to compile the FW!!!"
	current_path=`pwd`
	[ ! -d $current_path ] && echo "unknow erro, exit" && return
	[ ! -d $3 ] && echo "no such path, exit" && return
	[ "x$current_path" != "x$3" -a "x$current_path/" != "x$3" ] && cd $3 
	[ "x$3" != "x`pwd`" -a "x$3" != "x`pwd`/" ] && echo "cannot enter $3, exit." && return
	prepare
	expect_fun "$CMD_STRING"
	if [ "x`echo $?`" != "x0" ];then
		echo "git pull fail, retry..." 
		expect_fun "$CMD_STRING"
		[ "x`echo $?`" != "x0" ] && echo "git pull fail, exit" && return
	fi
	[ ! -d dni_home ] && echo "no dni_home, exit" && return
	cd dni_home && [ "x`echo $?`" != "x0" ] && echo "cannot enter dni_home, exit" && return 
	expect_fun "$CMD_STRING"
	if [ "x`echo $?`" != "x0" ];then
		echo "git pull fail, retry..." 
		expect_fun "$CMD_STRING"
		[ "x`echo $?`" != "x0" ] && echo "git pull dni_home fail, exit" && return
	fi
	cd - && make clean
	[ -f ./scripts/dni_defconfig_gen.sh ] && ./scripts/dni_defconfig_gen.sh
	rm -rf Compile-error-module.txt
	/usr/bin/make > Compile-log.txt
	while [ "x`echo $?`" != "x0" ]
	do
		collect_fail_module 
		/usr/bin/make > Compile-log.txt
	done
	exit
}

[ "x$1" = "x" -o "x$2" = "x" -o "x$3" = "x" ] && echo "Usage: $0 User_Name Password Project_Path" && exit
local_password=$2

CMD_STRING="$GIT_BIN pull"
compile_fun $1 $2 $3

