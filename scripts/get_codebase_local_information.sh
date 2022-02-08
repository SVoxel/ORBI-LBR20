#!/bin/sh

#This script is used to collect Orbi Maintrunk project's local file infomation.
#Attention: Scan root path should be your Maintrunk Project Buildroot path like 
#./Orbi-Desktop.git, which include /config, /kernel and /package dir.


echo "scan root path should be project's Maintrunk Project Build dir"
read -p "please input your scan root path (example: ./Orbi-Desktop.git) :" FilePath;
read -p "please input your result txt file (example: ./result.txt) :" ResultFile;

FilePath1=$FilePath/config
FilePath2=$FilePath/kernel
FilePath3=$FilePath/package

readfile() {
	for file in `ls $1`
	do
		if [ -d $1"/"$file ]
		then
			readfile $1"/"$file
		else
			echo $1"/"$file
		
		fi
	done
}

rm -f $ResultFile
for folder in $FilePath1 $FilePath2 $FilePath3
do
	echo read folder: $folder
	readfile $folder >> ./tmp.txt
done

sed "s%^$FilePath\/%%" ./tmp.txt | sed 's/\// /' | sed 's/\// /' | sed '/Makefile/'d | sed '/qca\-wifi\-fw\_dni/'d | sed '/\(\.png\|\.svg\|\.jpg\)/'d > $ResultFile

echo "package qca-wifi-fw_dni *" >> $ResultFile

rm -f ./tmp.txt
