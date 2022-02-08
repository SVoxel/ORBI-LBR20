#!/usr/bin/env bash
# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

TOPDIR=$1

[ ! -d $TOPDIR ] && echo ">>>>>>>>>> Aborting. Invalid TOP dirctory, Please Check on !!!!" && exit 1

echo ">>>>>>>>>>> Using TOPDIR: $TOPDIR"

[ ! -d $TOPDIR/dl ] && echo ">>>>>>>>>> Aborting. No find the ./dl directory, Please Check on !!!!" && exit 1
[ ! -d $TOPDIR/git_home ] && echo ">>>>>>>>>> Aborting. No find the ./git_home directory, Please Check on !!!!" && exit 1
[ ! -d $TOPDIR/orbi_maintrunk ] && echo ">>>>>>>>>> Aborting. No find the ./orbi_maintrunk directory, Please Check on !!!!" && exit 1

MT_CID=$(cat $TOPDIR/Maintrunk_Commit_ID | cut -f2 -d: | sed 's/ //g')

echo ">>>>>>>>>>> Using Maintrunk TAG: $MT_CID"

CMD_Run_Status=0
cd $TOPDIR/orbi_maintrunk; git-cat-file -e "$MT_CID"; CMD_Run_Status=$?;
if [ $CMD_Run_Status -ne 0 ]; then
	echo ">>>>>>>>>>> Aborting. No found the Maintrunk Commit ID(CID: $MT_CID)!!!!"
	exit 1
fi

if [ -f $TOPDIR/orbi_maintrunk_build_dir/Maintrunk_Commit_ID ]; then
	build_dir_cid="$(cat $TOPDIR/orbi_maintrunk_build_dir/Maintrunk_Commit_ID | cut -f2 -d:)"
	echo ">>>>>>>>>>> Build_Dir Maintrunk TAG: $build_dir_cid"
	if [ "$build_dir_cid" = "$MT_CID" ]; then
		echo ">>>>>>>>> Aborting. Same CID between Project Codebase and Build_dir, Ignore Prepare Action!!!!"
		exit 0
	fi
fi

[ -d $TOPDIR/orbi_maintrunk_build_dir ] && rm -rf $TOPDIR/orbi_maintrunk_build_dir

mkdir -p $TOPDIR/orbi_maintrunk_build_dir

cd $TOPDIR; git clone $TOPDIR/orbi_maintrunk orbi_maintrunk_build_dir

cd $TOPDIR/orbi_maintrunk_build_dir; git reset --hard "$MT_CID"

if [ -d $TOPDIR/maintrunk_patches ]; then
	for i in $TOPDIR/maintrunk_patches/*.patch; do
		[ -d "${i}" ] && echo "Ignoring subdirectory ${i}" && continue
		echo ""
		echo "Applying ${i}: "
		cat ${i} | patch -f -p1 -E -d $TOPDIR/orbi_maintrunk_build_dir
		if [ $? != 0 ] ; then
			echo "Patch failed!  Please fix $i!"
			exit 1
		fi
	done

	# Check for rejects...
	if [ "`find $TOPDIR/orbi_maintrunk_build_dir/ '(' -name '*.rej' -o -name '.*.rej' ')' -print`" ] ; then
		echo "Aborting.  Reject files found."
		exit 1
	fi

	# Remove backup files
	find $TOPDIR/orbi_maintrunk_build_dir/ '(' -name '*.orig' -o -name '.*.orig' ')' -exec rm -f {} \;

	cd $TOPDIR/orbi_maintrunk_build_dir; git add -A; git commit -s -m "Apply the Maintrunk Local Patches there"
fi

echo "Commit_ID:$MT_CID" > $TOPDIR/orbi_maintrunk_build_dir/Maintrunk_Commit_ID

cd $TOPDIR/orbi_maintrunk_build_dir; ln -s $TOPDIR/dl dl; ln -s $TOPDIR/git_home git_home; ln -s $TOPDIR dni_home;

