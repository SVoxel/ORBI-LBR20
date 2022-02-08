#!/bin/sh

#AT command fail it will send error msg like ERROR: error number code 
#in dnichat add ERROR: message to stderr 
#get-revision 		/tmp/mobile_output/revision
#get-sim-status		/tmp/mobile_output/sim_status
#get-adv-info		/tmp/mobile_output/adv_info

DEVPORT=/dev/ttyUSB2
OUTPUT_OPT="show"
OUTPUT_ITEM=""
DNICHAT=/usr/sbin/dnichat
MOBILE_TMP_DIR=/tmp/mobile_tmpfile
MOBILE_OUTPUT_DIR=/tmp/mobile_output
MB_PID=$$
CONFIG_CMD=/bin/config

usage(){
	echo -e "$0: is a simple tool \n \
	--get-revision			get the mobile FW version\n \
	--get-sim-status		get the mobile sim status\n \
	--get-network-registration	get the mobile network registration\n \
	--get-signal-strength		get the mobile signal strength\n \
	--pin-verify pincode		sim pin code verify\n \
	--puk-verify puk		sim puk code verify\n \
	power-on			lte power on\n \
	power-off			lte power off\n \
	--set-hot-swap			set hot swap pin in high level\n \
	--get-iccid			get sim iccid\n \
	--hot-swap-status		get hot swap status\n \
	--software-restart		software restart the module\n \
	--show-message			show sim card msg\n \
	--get-adv-info			get adv_info\n \
	--get-support-operator		get network available operator\n \
	--get-current-operator          get current network operator\n \
	--set-operator-selection	set operator selections\n \
	--def				factory default the module\n \
	--get-imei 			get lte module imei\n \
	--get-imsi 			get sim card imsi\n \
	--get-roamstate			get lte module roam setup\n \
	--set-roamstate			setup lte module roam on off\n \
	--get-pin-mode			get sim card in lock or unlock mode\n \
	--set-pin-mode			set sim card in lock or unlock mode\n \
	--get-pinpuk-count <get pin|puk>get pin or puk less verify count\n \
	--change-passwd			change pin code when pin mode is lock\n \
	--set-mbscanmode		set lte module scan network mode lte wcdma umts...\n \
	--del-sms 			del the sms through index save in lte module\n \
	--get-sim-number		get the sim card own phone number\n \
	--get-pa-temp			get the lte module pa temperature\n \
	--get-band-freq                 get LTE band and frequency\n \
	--eg18-upgrade	<file path>	do the eg18 upgrade\n \
	--get-current-roaming		get current module is in roaming status or not

	option: \n \
		get  item		output the item\n "  >&2	
}

get_revision(){
	$DNICHAT $DEVPORT ATI 2> $MOBILE_TMP_DIR/Revision.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/Revision.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/Revision.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/Revision.error.$MB_PID
		exit 1
	fi
	grep Revision: $MOBILE_TMP_DIR/Revision.error.$MB_PID |awk -F ' ' '{print $2}' > $MOBILE_TMP_DIR/Revision.$MB_PID
	cp $MOBILE_TMP_DIR/Revision.$MB_PID $MOBILE_OUTPUT_DIR/Revision
	cat $MOBILE_TMP_DIR/Revision.$MB_PID >&1

	rm $MOBILE_TMP_DIR/Revision.$MB_PID
	rm $MOBILE_TMP_DIR/Revision.error.$MB_PID
	
}
# $1 pin or puk, $2 pin or puk;same return 0 diff 1
comp_pinpuk()
{
	case $1 in
		pin)
		local LAST_PIN=$(config get wan2.pin_code)
		if [ "x$LAST_PIN" == "x$2" ];then
			return 0
		else
			return 1
		fi
		;;
		puk)
		local LAST_PUK=$(config get wan2.puk_code)
		if [ "x$LAST_PUK" == "x$2" ];then
			return 0
		else
			return 1
		fi
		;;
		*)
			return 1;;
	esac
}
# $1 pin or puk, $2 pin or puk;same return 0 diff 1
valid_pinpuk()
{
	local PINPUK=$2
	case $1 in
		pin)
		if [ ${#PINPUK} -lt 4 ] || [ ${#PINPUK} -gt 8 ] || [ "x`echo $PINPUK |grep ^[0-9]*$`" == "x" ];then
			return 1
		fi
		return 0
		;;
		puk)
		if [ ${#PINPUK} -ne 8 ] || [ "x`echo $PINPUK |grep ^[0-9]*$`" == "x" ];then
			return 1
		fi
		return 0
		;;
		*)
			return 1;;
	esac
}
#mbctl.sh --pin-verify 1234
pin_verify(){
	PINCODE=$2

	valid_pinpuk "pin" "$PINCODE"
	#get and verify the pin code is valid?
	if [ $? -ne 0 ];then
		echo "pin code rule is error, you need input 4 - 8 number" >&2
		echo fail >&1
		exit 1
	fi
	$DNICHAT $DEVPORT AT+CPIN=$PINCODE 2> $MOBILE_TMP_DIR/pin-verify.error 
	exit_num=$?

	sim_status=`$DNICHAT $DEVPORT AT+CPIN? 2>&1|grep "+CPIN:"|awk -F ": " '{print $2}'|awk -F '\r' '{print $1}'`
	if [ "x$sim_status" == "xREADY" ];then
		comp_pinpuk "pin" "$PINCODE"
		if [ $? -eq 0 ];then
			echo "pin verify success" >&1
			exit 0
		fi
	fi
	if [ $exit_num -ne 0 ];then
		echo fail >&1
		echo "dnichat exit num: $exit_num" >&2
		exit $exit_num
	fi
	if [ "x`grep OK $MOBILE_TMP_DIR/pin-verify.error`" != "x" ];then
		$CONFIG_CMD set wan2.pin_code=$PINCODE
		echo "pin verify success" >&1
		exit 0
	fi
	grep "ERROR" $MOBILE_TMP_DIR/pin-verify.error >&2
	echo fail >&1
	rm $MOBILE_TMP_DIR/pin-verify.error
}

#mbctrl.sh --puk-verify 12345678
puk_verify(){

	PUKCODE=$2
	PINCODE=$3
	valid_pinpuk "puk" "$PUKCODE"
	#get and verify the puk code is valid?
	if [ $? -ne 0 ];then
		echo "puk code rule is error, you need input 8 number" >&2
	fi
	valid_pinpuk "pin" "$PINCODE"
	if [ $? -ne 0 ];then
		echo "new pin code rule is error, you need input 4 - 8 number" >&2
		echo fail >&1
		exit 1
	fi
	sim_status=`$DNICHAT $DEVPORT AT+CPIN? 2>&1|grep "+CPIN:"|awk -F ": " '{print $2}'|awk -F '\r' '{print $1}'`
	if [ "x$sim_status" != "xSIM PUK" ];then
		comp_pinpuk "puk" "$PUKCODE"
		if [ $? -eq 0 ];then
			echo "puk verify success" >&1
			exit 0
		fi
		#if is not in SIM PUK will let SIM PUK2 less count reduce
		#but it will be ready auto, can not know what about sim puk2
		#did not know what about it so when not in SIM PUK will return fail not do verify
		echo "fail" >&1
		exit 1
	fi
	$DNICHAT $DEVPORT AT+CPIN=\"$PUKCODE\",\"$PINCODE\" 2> $MOBILE_TMP_DIR/puk-verify.error
	exit_num=$?
	if [ $exit_num -ne 0 ];then
		echo fail >&1
		echo "dnichat exit num: $exit_num" >&2
		exit $exit_num
	fi
	if [ "x`grep OK $MOBILE_TMP_DIR/puk-verify.error`" != "x" ];then
		$CONFIG_CMD set wan2.pin_code=$PINCODE
		$CONFIG_CMD set wan2.puk_code=$PUKCODE
		echo "puk verify success" >&1
		exit 0
	fi
	grep "ERROR" $MOBILE_TMP_DIR/puk-verify.error >&2
	echo fail >&1
	rm $MOBILE_TMP_DIR/puk-verify.error

}

get_sim_status(){
	$DNICHAT $DEVPORT AT+QSIMSTAT? 2> $MOBILE_TMP_DIR/sim_status.error.$MB_PID
	insert_status=`grep "+QSIMSTAT:" $MOBILE_TMP_DIR/sim_status.error.$MB_PID |awk -F ',' '{print $2}'|awk -F '\r' '{print $1}'`
	if [ $insert_status -eq 0 ];then
		echo "UNINSERT" > $MOBILE_TMP_DIR/sim_status.$MB_PID
	else
		$DNICHAT $DEVPORT AT+CPIN? 2> $MOBILE_TMP_DIR/sim_status.error.$MB_PID
		exit_num=$?
		if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/sim_status.error.$MB_PID`" != "x" ];then
			echo "dnichat exit num: $exit_num" >&2
			grep ERROR $MOBILE_TMP_DIR/sim_status.error.$MB_PID >&2
			rm $MOBILE_TMP_DIR/sim_status.error.$MB_PID
			exit 1
		fi
		grep "+CPIN:" $MOBILE_TMP_DIR/sim_status.error.$MB_PID |awk -F ": " '{print $2}'|awk -F '\r' '{print $1}' > $MOBILE_TMP_DIR/sim_status.$MB_PID
	fi
	cp $MOBILE_TMP_DIR/sim_status.$MB_PID $MOBILE_OUTPUT_DIR/sim_status

	cat $MOBILE_TMP_DIR/sim_status.$MB_PID >&1
	rm $MOBILE_TMP_DIR/sim_status.$MB_PID
	rm $MOBILE_TMP_DIR/sim_status.error.$MB_PID
}
#AT+CREG=2
get_network_registration(){
	get_user_option $@
	$DNICHAT $DEVPORT AT+CREG? 2> $MOBILE_TMP_DIR/network_registration.error.$MB_PID
	
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/network_registration.error.$MB_PID`" != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/network_registration.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/network_registration.error.$MB_PID
		exit 1
	fi
	grep +CREG: $MOBILE_TMP_DIR/network_registration.error.$MB_PID |awk -F ':' '{print $2}'|awk -F '\r' '{print $1}'|sed 's:,:\n:g' > $MOBILE_TMP_DIR/network_registration.attach.$MB_PID
	
	echo "{" > $MOBILE_TMP_DIR/network_registration.$MB_PID
	awk 'NR==2{print "\tstate:\t"$0}' $MOBILE_TMP_DIR/network_registration.attach.$MB_PID >> $MOBILE_TMP_DIR/network_registration.$MB_PID
	awk 'NR==3{print "\tlac:\t"$0}' $MOBILE_TMP_DIR/network_registration.attach.$MB_PID >> $MOBILE_TMP_DIR/network_registration.$MB_PID
	awk 'NR==4{print "\tcellid:\t"$0}' $MOBILE_TMP_DIR/network_registration.attach.$MB_PID >> $MOBILE_TMP_DIR/network_registration.$MB_PID
	awk 'NR==5{print "\tact:\t"$0}' $MOBILE_TMP_DIR/network_registration.attach.$MB_PID >> $MOBILE_TMP_DIR/network_registration.$MB_PID
	echo "}" >> $MOBILE_TMP_DIR/network_registration.$MB_PID
	case $OUTPUT_OPT in 
		get)
		grep $OUTPUT_ITEM $MOBILE_TMP_DIR/network_registration.$MB_PID|awk -F ":\t" '{print $2}' >&1
		;;
		*)
		cat $MOBILE_TMP_DIR/network_registration.$MB_PID >&1
		;;	
	esac
	cp $MOBILE_TMP_DIR/network_registration.$MB_PID $MOBILE_OUTPUT_DIR/network_registration	

	rm $MOBILE_TMP_DIR/network_registration.attach.$MB_PID
	rm $MOBILE_TMP_DIR/network_registration.error.$MB_PID
	rm $MOBILE_TMP_DIR/network_registration.$MB_PID

}
#spec Rev14 page488 signal bar
judge_signal_bar(){
	local RANGE_LTE="[-118 -118,-112 -112,-106 -106,-100 -100]"
	local RANGE_3G="[-106 -106,-100 -100,-90 -90,-80 -80]"
	local RANGE_2G="[-104 -104,-98 -98,-89 -89,-80 -80]"
	
	if [ $1 == 0 ];then
		local RANGE="$RANGE_2G"
	elif [ $1 == 1 ];then	
		RANGE="$RANGE_3G"
	else
		RANGE="$RANGE_LTE"
	fi
	local BAR_NUM=0
	local i=1
	local comp=$2
	for val in $RANGE
	do
		if [ "`echo $val|grep '\['`" != "" ];then
			right=`echo $val|sed 's:\[::g'`
			if [ $comp -le $right ];then
				BAR_NUM=$i
				break
			fi
		fi
		if [ "`echo $val|grep ,`" != "" ];then
			left=`echo $val|awk -F ',' '{print $1}'`
			right=`echo $val|awk -F ',' '{print $2}'`
			if [ $comp -gt $left ] && [ $comp -le $right ];then
				BAR_NUM=$i
				break
			fi
		fi
		if [ "`echo $val|grep '\]'`" != "" ];then
			left=`echo $val|sed 's:\]::g'`
			if [ $comp -gt $right ];then
				BAR_NUM=$i
				break
			fi
		fi
		i=$(($i+1))
	done
	echo $BAR_NUM > $MOBILE_TMP_DIR/signal_bar_num.$MB_PID
	cp $MOBILE_TMP_DIR/signal_bar_num.$MB_PID $MOBILE_OUTPUT_DIR/signal_bar_num
	cat $MOBILE_TMP_DIR/signal_bar_num.$MB_PID >&1
	rm $MOBILE_TMP_DIR/signal_bar_num.$MB_PID
}
ATTACH=""
attach_list(){
	echo "$1"|sed 's: :\n:g' > $3.tmp
	echo "$2"|sed 's:,:\n:g' > $3.attach
	awk 'NR==FNR{a[i]=$0;i++}NR>FNR{print "\t"a[j]":\t"$0;j++}' $3.tmp $3.attach|sed 's:%::g' >> $3
	rm $3.tmp
	rm $3.attach
}
combine_list(){
	local IS_LTE=$1
	VAL_LST=$2
	SORT_LST=$3
	FILE_NAME=$4
	attach_list "$VAL_LST" "$ATTACH" "$FILE_NAME.$MB_PID"
	#LTE
	if [ $IS_LTE == 2 ];then
		echo -e "\trscp:\t\n\tecio:\t" >> $FILE_NAME.$MB_PID
	#3G add missing LTE null items
	else
		echo -e "\trssi:\t\n\trsrp:\t\n\trsrq:\t\n\tsinr:\t" >> $FILE_NAME.$MB_PID
	fi
	#sort in common index
	echo "{" > $FILE_NAME
	for sort_ind in $SORT_LST
	do 
		grep $sort_ind $FILE_NAME.$MB_PID|sed 's:%:,:g' >> $FILE_NAME
	done
	echo "}" >> $FILE_NAME
	rm $FILE_NAME.$MB_PID
}
#AT+QENG
#--get-signal-strength get rssi will create /tmp/mobile_output/signal_bar_num record bar num 
get_signal_strength(){
	#lte list
	VAL_LTE="celltype state mode is_tdd mcc mnc cellid pcid earfcn freq_band_ind ul_bandwith dl_bandwith lac rsrp rsrq rssi sinr cqi"
	#3g list
	VAL_3G="celltype state mode mcc mnc lac cellid uarfcn psc rac rscp ecio phych"
	#sig list
	SIG_LST="rssi rsrp rsrq sinr rscp ecio"
	local RSSI=-75
	local IS_LTE=2
	get_user_option $@
	$DNICHAT $DEVPORT "AT+QNWINFO" 2> $MOBILE_TMP_DIR/signal-strength.error.$MB_PID

	exit_num=$?
	if [ $exit_num -ne 0 ];then
		echo "dnichat exit num: $exit_num" >&2
		rm $MOBILE_TMP_DIR/signal-strength.error.$MB_PID
		exit 1
	fi

	CONNECT=`grep +QNWINFO: $MOBILE_TMP_DIR/signal-strength.error.$MB_PID|awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}'|awk -F ',' '{print $1}'|sed 's:\"::g'`
	if [ "$CONNECT" == "NONE"  ] || [ "$CONNECT" == "No Service" ];then
		echo "0" >&1
		echo "0" > $MOBILE_OUTPUT_DIR/signal_bar_num
		echo "No Service" > $MOBILE_OUTPUT_DIR/signal_strength
		rm $MOBILE_TMP_DIR/signal-strength.error.$MB_PID
		exit 0
	elif [ "$CONNECT" == "TDD LTE" ] || [ "$CONNECT" == "FDD LTE" ];then
		IS_LTE=2
	elif [ "$CONNECT" == "GSM" ] || [ "$CONNECT" == "GPRS" ] || [ "$CONNECT" == "EDGE" ] || [ x`echo "$CONNECT"|grep CDMA1X` != "x" ];then
		IS_LTE=0
	else
		IS_LTE=1
	fi
	if [ $IS_LTE == 0 ];then
		#for 2g
		$DNICHAT $DEVPORT "AT+CSQ" 2> $MOBILE_TMP_DIR/signal-strength.error.$MB_PID
		ASU=`grep "+CSQ:" $MOBILE_TMP_DIR/signal-strength.error.$MB_PID|awk -F ': ' '{print $2}'|awk -F ',' '{print $1}'`
		if [ -n $ASU ];then
			let RSSI=2*$ASU-113
		fi
		echo -e "\trssi:\t${RSSI}" > $MOBILE_TMP_DIR/signal_strength.$MB_PID
	else	
		#lte and 3g
		$DNICHAT $DEVPORT "AT+QENG=\"servingcell\"" 2> $MOBILE_TMP_DIR/signal-strength.error.$MB_PID
		exit_num=$?
		if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/signal-strength.error.$MB_PID` != "x" ];then
			echo "dnichat exit num: $exit_num" >&2
			grep ERROR $MOBILE_TMP_DIR/signal-strength.error.$MB_PID >&2
			rm $MOBILE_TMP_DIR/signa-strength.error.$MB_PID
			exit 1
		fi
		ATTACH=$(grep "+QENG:" $MOBILE_TMP_DIR/signal-strength.error.$MB_PID |awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}')
		if [ "x`echo $ATTACH|grep '"SEARCH"'`" != "x" ];then 
			echo "{" > $MOBILE_TMP_DIR/signal_strength.$MB_PID
			echo -e "\trssi:\t\n\trsrp:\t\n\trsrq:\t\n\tsinr:\t\n\trscp:\t\n\tecio\t" >> $MOBILE_TMP_DIR/signal_strength.$MB_PID
			echo "}" >> $MOBILE_TMP_DIR/signal_strength.$MB_PID
		else
			if [ $IS_LTE == 1 ];then
				VAL_LST="$VAL_3G"
			else
				VAL_LST="$VAL_LTE"
			fi
			combine_list $IS_LTE "$VAL_LST" "$SIG_LST" "$MOBILE_TMP_DIR/signal_strength.$MB_PID"
		fi
		
	fi
	case $OUTPUT_OPT in 
		get)
		if [ $OUTPUT_ITEM == rssi ];then
			RSSI=`grep $OUTPUT_ITEM $MOBILE_TMP_DIR/signal_strength.$MB_PID|awk -F ":\t" '{print $2}'`
			[ $IS_LTE == 1 ]&& RSSI=`grep rscp $MOBILE_TMP_DIR/signal_strength.$MB_PID|awk -F ":\t" '{print $2}'`
			if [ -n $RSSI ];then
	       			judge_signal_bar $IS_LTE $RSSI
			fi
		else
			grep $OUTPUT_ITEM $MOBILE_TMP_DIR/signal_strength.$MB_PID|awk -F ":\t" '{print $2}' >&1
		fi
		;;
		*)
		cat $MOBILE_TMP_DIR/signal_strength.$MB_PID >&1
		;;
	esac
	cp $MOBILE_TMP_DIR/signal_strength.$MB_PID $MOBILE_OUTPUT_DIR/signal_strength 
	rm $MOBILE_TMP_DIR/signal_strength.$MB_PID
	rm $MOBILE_TMP_DIR/signal-strength.error.$MB_PID
}

#adv-info need AT+QENG AT+QNWINFO AT+CGPADDR AT+QNWINFO
get_adv_info(){
	#prefix
	VAL_PREFIX="psservice pdp_addr networkselectmode plmn radioband channelid"
	#lte list
	VAL_LTE="celltype state mode is_tdd mcc mnc non-pcid cellid earfcn freq_band_ind ul_bandwith dl_bandwith lac rsrp rsrq rssi sinr cqi"
	#3g list
	VAL_3G="celltype state mode mcc mnc lac cellid uarfcn psc rac rscp ecio phych"
	#final list format
	ADV_INFO="psservice pdp_addr networkselectmode plmn radioband channelid state mcc mnc cellid lac rsrp rsrq rssi sinr cqi rscp ecio"
	SEARCH="%,%,%,%,%,%,\"SEARCH\",%,%,%,%,%,%,%,%,%,%,%"
	NO_CONN="%,%,%,%,%,%,\"LIMSRV\",%,%,%,%,%,%,%,%,%,%,%"
	get_user_option $@
	local IS_LTE=2
	$DNICHAT $DEVPORT "AT+CGATT?" 2> $MOBILE_TMP_DIR/adv-info.error.$MB_PID
	if [ $? -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID >&2
		cgatt="%"
	else
		cgatt="$(grep "+CGATT:" $MOBILE_TMP_DIR/adv-info.error.$MB_PID |awk -F ' ' '{print $2}'|awk -F '\r' '{print $1}')"
	fi
	$DNICHAT $DEVPORT "AT+CGPADDR=1" 2> $MOBILE_TMP_DIR/adv-info.error.$MB_PID
	if [ $? -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID >&2
		cgpaddr="%"
	else
		cgpaddr="$(grep "+CGPADDR:" $MOBILE_TMP_DIR/adv-info.error.$MB_PID |awk -F '\"' '{print $2}'|awk -F ',' '{print $1}')"
	fi
	$DNICHAT $DEVPORT "AT+QNWINFO" 2> $MOBILE_TMP_DIR/adv-info.error.$MB_PID
	if [ $? -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID` != "x" ] || [ "x`grep 'No Service' $MOBILE_TMP_DIR/adv-info.error.$MB_PID`" != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID >&2
		qnwinfo="%,%,%,%,%"
	else
		qnwinfo="$(grep "+QNWINFO:" $MOBILE_TMP_DIR/adv-info.error.$MB_PID |awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}')"
	fi
	CONNECT=`grep +QNWINFO: $MOBILE_TMP_DIR/adv-info.error.$MB_PID|awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}'|awk -F ',' '{print $1}'|sed 's:\"::g'`
	if [ "$CONNECT" == "NONE"  ] || [ "$CONNECT" == "No Service" ];then
		echo "No Service" >&2
		echo "{" > $MOBILE_TMP_DIR/adv_info.$MB_PID
		attach_list "$ADV_INFO" "$NO_CONN" "$MOBILE_TMP_DIR/adv_info.$MB_PID"
		echo "}" >> $MOBILE_TMP_DIR/adv_info.$MB_PID
	else
		if [ "$CONNECT" == "TDD LTE" ] || [ "$CONNECT" == "FDD LTE" ];then
			IS_LTE=2
		elif [ "$CONNECT" == "GSM" ] || [ "$CONNECT" == "GPRS" ] || [ "$CONNECT" == "EDGE" ] || [ x`echo "$CONNECT"|grep CDMA1X` != "x" ];then
			IS_LTE=0
		else
			IS_LTE=1
		fi
		$DNICHAT $DEVPORT "AT+QENG=\"servingcell\"" 2> $MOBILE_TMP_DIR/adv-info.error.$MB_PID
		if [ $? -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID` != "x" ];then
			grep ERROR $MOBILE_TMP_DIR/adv-info.error.$MB_PID >&2
			exit 1
		fi
		qeng_val="`grep "+QENG:" $MOBILE_TMP_DIR/adv-info.error.$MB_PID |awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}'`"
		if [ "x`echo $qeng_val|grep '"SEARCH"'`" != "x" ];then 
			echo "{" > $MOBILE_TMP_DIR/adv_info.$MB_PID
			attach_list "$ADV_INFO" "$SEARCH" "$MOBILE_TMP_DIR/adv_info.$MB_PID"
			echo "}" >> $MOBILE_TMP_DIR/adv_info.$MB_PID
		else
			ATTACH="$cgatt,$cgpaddr,$qnwinfo,$qeng_val"
			if [ $IS_LTE == 1 ];then
				VAL_LST="$VAL_PREFIX $VAL_3G"
			else
				VAL_LST="$VAL_PREFIX $VAL_LTE"
			fi
			combine_list $IS_LTE "$VAL_LST" "$ADV_INFO" "$MOBILE_TMP_DIR/adv_info.$MB_PID"
		fi
	fi
	case $OUTPUT_OPT in 
		get)
		grep $OUTPUT_ITEM $MOBILE_TMP_DIR/adv_info.$MB_PID|awk -F ":\t" '{print $2}' >&1
		;;
		*)
		cat $MOBILE_TMP_DIR/adv_info.$MB_PID >&1
		;;
	esac
	cp $MOBILE_TMP_DIR/adv_info.$MB_PID $MOBILE_OUTPUT_DIR/adv_info

	rm $MOBILE_TMP_DIR/adv_info.$MB_PID
	rm $MOBILE_TMP_DIR/adv-info.error.$MB_PID
}

#get current available operator
get_support_operator(){
	#save the last quectel-CM paramters firstly
	local qcm_opt=$(ps ww|grep quectel-CM|grep -v grep|awk -F '-CM' '{print $2}')
	echo "[MB] do operator scan need stop quectel-CM firstly" > /dev/console
	local qcm_status=$(ps ww|grep quectel-CM|grep -v grep)
	if [ "${qcm_status}" != "" ];then
		killall quectel-CM
	fi
	$DNICHAT $DEVPORT "AT+COPS=?" 2> $MOBILE_TMP_DIR/get-operator.error.$MB_PID

	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/get-operator.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/get-operator.error.$MB_PID >&2
		#if happend error need echo null file
		echo "" > $MOBILE_OUTPUT_DIR/operator_available
		rm $MOBILE_TMP_DIR/get-operator.error.$MB_PID
		echo "[MB] do operator scan restart quectel-CM later" > /dev/console
		if [ "${qcm_status}" != "" ];then
			quectel-CM ${qcm_opt} &		
		fi
		exit 1
	fi
	grep "+COPS:" $MOBILE_TMP_DIR/get-operator.error.$MB_PID |awk -F ': ' '{print $2}'|awk -F '\r' '{print $1}'|sed 's:),*,:)\n:g' > $MOBILE_TMP_DIR/operator_available.$MB_PID
	cat $MOBILE_TMP_DIR/operator_available.$MB_PID >&1	
	cp $MOBILE_TMP_DIR/operator_available.$MB_PID $MOBILE_OUTPUT_DIR/operator_available

	rm $MOBILE_TMP_DIR/operator_available.$MB_PID
	rm $MOBILE_TMP_DIR/get-operator.error.$MB_PID
	echo "[MB] do operator scan restart quectel-CM later" > /dev/console
	if [ "${qcm_status}" != "" ];then
		quectel-CM ${qcm_opt} &		
	fi
}

#get current operator
get_current_operator(){
	$DNICHAT $DEVPORT "AT+QSPN" 2> $MOBILE_TMP_DIR/get-current-operator.error.$MB_PID
	local operator=""
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/get-current-operator.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/get-current-operator.error.$MB_PID >&2
		echo "\"\"" > $MOBILE_TMP_DIR/current_operator.$MB_PID
	else
		operator=`grep "+QSPN:" $MOBILE_TMP_DIR/get-current-operator.error.$MB_PID |awk -F ':' '{print $2}'|awk -F '\r' '{print $1}'|awk -F ',' '{print $2}'`
		if [ "x$operator" == "x" ];then
			echo "\"\"" > $MOBILE_TMP_DIR/current_operator.$MB_PID
		else
	       		echo $operator > $MOBILE_TMP_DIR/current_operator.$MB_PID
		fi
	fi
	cat $MOBILE_TMP_DIR/current_operator.$MB_PID >&1
	cp $MOBILE_TMP_DIR/current_operator.$MB_PID $MOBILE_OUTPUT_DIR/current_operator

	rm $MOBILE_TMP_DIR/current_operator.$MB_PID
	rm $MOBILE_TMP_DIR/get-current-operator.error.$MB_PID
}

#set mannual operator
set_operator_selection(){

	MODE=$2
	FORMAT=$3
	OPERATOR=$4
	TECH_SELECT=$5

	$DNICHAT $DEVPORT "AT+COPS=$MODE,$FORMAT,\"$OPERATOR\",$TECH_SELECT" 2> $MOBILE_TMP_DIR/set-operator.error
	exit_num=$?

	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/set-operator.error` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/set-operator.error >&2
		echo fail >&1
		rm $MOBILE_TMP_DIR/set-operator.error
		exit 1
	fi
	rm $MOBILE_TMP_DIR/set-operator.error
	echo "set operator success" >&1
	
}

show_message(){
	$DNICHAT $DEVPORT "AT+CMGF=0" 2>/dev/null
	$DNICHAT $DEVPORT "AT+CMGL=4" 2> $MOBILE_OUTPUT_DIR/sms_info
	[ $? -ne 0 ]&& exit $?
	cat $MOBILE_OUTPUT_DIR/sms_info >&1

}

eg18_upgrade(){
	local count=0
	$CONFIG_CMD set mb_upgrade_process=running
	while true
	do
		let count+=1
		$DNICHAT $DEVPORT AT+QFDEL=\"*\" 2> $MOBILE_TMP_DIR/mobile_upgrade.error
		if [ "x`grep OK $MOBILE_TMP_DIR/mobile_upgrade.error`" != "x" ];then
			break;
		fi
		if [ $count -gt 3 ];then
			echo "del_fail" > $MOBILE_OUTPUT_DIR/mobile_upgrade
			break;
		fi
		sleep 10
	done
	if [ -n $1 ] && [ -e $MOBILE_TMP_DIR/$1 ];then
		FW_SIZE=`ls -l $MOBILE_TMP_DIR/$1|awk '{print $5}'`
		$DNICHAT $DEVPORT AT+QFUPL=\"UFS:$1\",$FW_SIZE,20 2> $MOBILE_TMP_DIR/mobile_upgrade.error
		#after remove the FOTA file it use too large memory
		echo "remove lte fw file:$MOBILE_TMP_DIR/$1" > /dev/console
		rm $MOBILE_TMP_DIR/$1
		if [ "x`grep '\"END\",0' $MOBILE_TMP_DIR/mobile_upgrade.error`" != "x" ];then
			echo "success" > $MOBILE_OUTPUT_DIR/mobile_upgrade
			$CONFIG_CMD set mb_upgrade_process=success
		else
			echo "fail" > $MOBILE_OUTPUT_DIR/mobile_upgrade
			$CONFIG_CMD set mb_upgrade_process=fail
		fi
	else
	        $CONFIG_CMD set mb_upgrade_process=fail	
		echo "file not found" > $MOBILE_OUTPUT_DIR/mobile_upgrade
		
	fi
}

LTE_RST=28
LTE_PWRKEY=29
LTE_USB_BOOT=30
LTE_PWM=31

lte_pwroff()
{
	# Per HW Datasheet, 3.7.2.1- Turn off the module using PWRKEY
	# Drive PWRKEY to low for at least 800ms, then release it
	echo "[MBCTL]: power off LTE module"
	gpio -n gpio_write -c $LTE_PWRKEY -s 0
	sleep 1
	gpio -n gpio_write -c $LTE_PWRKEY -s 1
	echo "Wait ~8s for LTE shutdown(Avoid damage its internal flash)"
	sleep 10
	echo "."
}

lte_pwron()
{
	gpio -n gpio_write -c $LTE_RST -s 0
	gpio -n gpio_write -c $LTE_PWRKEY -s 0
	gpio -n gpio_write -c $LTE_USB_BOOT -s 0
	gpio -n gpio_write -c $LTE_PWM -s 0
	sleep 1
	gpio -n gpio_write -c $LTE_PWM -s 1
	gpio -n gpio_write -c $LTE_PWRKEY -s 1
	gpio -n gpio_write -c $LTE_RST -s 1
	sleep 1
	gpio -n gpio_write -c $LTE_PWRKEY -s 0
	sleep 1
	gpio -n gpio_write -c $LTE_PWRKEY -s 1
}

#set hot swap in high level
set_hot_swap(){
	$DNICHAT $DEVPORT AT+QSIMDET=1,1 2> $MOBILE_TMP_DIR/set-hot-swap.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/set-hot-swap.error`" != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/set-hot-swap.error >&2
		echo "dnichat exit num: $exit_num" >&2
		rm $MOBILE_TMP_DIR/set-hot-swap.error
		exit 1
	fi
}
#get hot swap status return enable disable
get_hot_swap_status(){
	local status=disable
	$DNICHAT $DEVPORT AT+QSIMDET? 2> $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID`" != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID
		exit 1
	fi
	status=`grep "+QSIMDET:" $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID | awk -F ': ' '{print $2}' |awk -F '\r' '{print $1}'`
	if [ $status == "1,1" ];then
		echo "enable" > $MOBILE_TMP_DIR/hot_swap_status.$MB_PID
	else
		echo "disable" > $MOBILE_TMP_DIR/hot_swap_status.$MB_PID
	fi
	cat $MOBILE_TMP_DIR/hot_swap_status.$MB_PID >&1
	cp $MOBILE_TMP_DIR/hot_swap_status.$MB_PID $MOBILE_OUTPUT_DIR/hot_swap_status

	rm $MOBILE_TMP_DIR/hot_swap_status.$MB_PID
	rm $MOBILE_TMP_DIR/hot-swap-status.error.$MB_PID

}
#get iccid
get_iccid(){
	$DNICHAT $DEVPORT AT+QCCID 2> $MOBILE_TMP_DIR/get-iccid.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/get-iccid.error.$MB_PID`" != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/get-iccid.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/get-iccid.error.$MB_PID
		exit 1
	fi
	grep "+QCCID:" $MOBILE_TMP_DIR/get-iccid.error.$MB_PID | awk -F ': ' '{print $2}' |awk -F '\r' '{print $1}' > $MOBILE_TMP_DIR/iccid.$MB_PID
	cat $MOBILE_TMP_DIR/iccid.$MB_PID >&1
	cp $MOBILE_TMP_DIR/iccid.$MB_PID $MOBILE_OUTPUT_DIR/iccid

	rm $MOBILE_TMP_DIR/iccid.$MB_PID
	rm $MOBILE_TMP_DIR/get-iccid.error.$MB_PID


}
#mobile software restart
software_restart(){
	$DNICHAT $DEVPORT AT+CFUN=0 2> $MOBILE_TMP_DIR/software-restart.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/software-restart.error`" != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/software-restart.error >&2
		rm $MOBILE_TMP_DIR/software-restart.error
		exit 1
	fi
	sleep 1
	$DNICHAT $DEVPORT AT+CFUN=1 2>> $MOBILE_TMP_DIR/software-restart.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/software-restart.error`" != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/software-restart.error >&2
		rm $MOBILE_TMP_DIR/software-restart.error
		exit 1
	fi
}

# Factory defualt the module
factory_default()
{
	$DNICHAT $DEVPORT at+qprtpara=3

	#Then reboot the module
	lte_pwron
}

# for IMEI check 
# Returns Luhn checksum for supplied sequence
luhn_checksum() {
	sequence="$1"
	# sequence="${sequence//[^0-9]}"
	# numbers only plz
	if [ "$(echo "$sequence"|grep '^[0-9]*$')" == "" ];then
		return 1
	fi
        checksum=0
        table="0 2 4 6 8 1 3 5 7 9"
	local index=0

        # Quicker to work with even number of digit
        # prepend a "0" to sequence if uneven
        i=${#sequence}
        if [ $(($i % 2)) -ne 0 ]; then
        	sequence="0$sequence"
		i=$((i + 1))
        fi

        while [ $i -ne 0 ];
        do
            	# sum up the individual digits, do extra stuff w/every other digit
        	checksum="$(($checksum + ${sequence:$((i - 1)):1}))" # Last digit
	        # for every other digit, double the value before adding the digit
            	# if the doubled value is over 9, subtract 9
		index="${sequence:$((i - 2)):1}"
		j=0
		for map_tab in $table
		do
			if [ "$index" == "$j" ];then
				break
			fi
			j=$((j + 1))
		done
	        checksum="$(($checksum + ${map_tab}))" # Second to last digit
            	i=$((i - 2))
        done
        checksum="$(($checksum % 10))" # mod 10 the sum to get single digit checksum
        echo "$checksum"
}
# Tests if last digit is the correct Luhn check digit for the sequence
# Returns true if valid, false if not
luhn_test() {
        if [ "$(luhn_checksum $1)" == "0" ]; then
            	return 0
        else
	        return 1
        fi
}
get_imei(){
	$DNICHAT $DEVPORT AT+GSN 2> $MOBILE_TMP_DIR/imei.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/imei.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/imei.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/imei.error.$MB_PID
		exit 1
	fi
	local LINE_NUM=$(grep '^AT+GSN*' -rn $MOBILE_TMP_DIR/imei.error.$MB_PID|awk -F ':' '{print $1}')
	let LINE_NUM+=1
	val_imei="$(awk -v var="$LINE_NUM" 'NR==var' $MOBILE_TMP_DIR/imei.error.$MB_PID |awk -F '\r' '{print $1}'|grep '^[0-9]*$')"
	if [ "$2" == "check" ];then
		if luhn_test "$val_imei";then
			echo "$val_imei" > $MOBILE_TMP_DIR/imei.$MB_PID
		else
			echo "Invalid-${val_imei}" > $MOBILE_TMP_DIR/imei.$MB_PID
		fi
	else
		echo "$val_imei" > $MOBILE_TMP_DIR/imei.$MB_PID
	fi
	cp $MOBILE_TMP_DIR/imei.$MB_PID $MOBILE_OUTPUT_DIR/imei
	cat $MOBILE_TMP_DIR/imei.$MB_PID >&1

	rm $MOBILE_TMP_DIR/imei.$MB_PID
	rm $MOBILE_TMP_DIR/imei.error.$MB_PID
	
}

get_imsi(){
	$DNICHAT $DEVPORT AT+CIMI 2> $MOBILE_TMP_DIR/imsi.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/imsi.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/imsi.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/imsi.error.$MB_PID
		exit 1
	fi
	local LINE_NUM=$(grep '^AT+CIMI*' -rn $MOBILE_TMP_DIR/imsi.error.$MB_PID|awk -F ':' '{print $1}')
	let LINE_NUM+=1
	awk -v var="$LINE_NUM" 'NR==var' $MOBILE_TMP_DIR/imsi.error.$MB_PID |awk -F '\r' '{print $1}'|grep '^[0-9]*$' > $MOBILE_TMP_DIR/imsi.$MB_PID
	cp $MOBILE_TMP_DIR/imsi.$MB_PID $MOBILE_OUTPUT_DIR/imsi
	cat $MOBILE_TMP_DIR/imsi.$MB_PID >&1

	rm $MOBILE_TMP_DIR/imsi.$MB_PID
	rm $MOBILE_TMP_DIR/imsi.error.$MB_PID
	
}
get_roamstate(){
	$DNICHAT $DEVPORT AT+QCFG=\"roamservice\" 2> $MOBILE_TMP_DIR/roam.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/roam.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/roam.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/roam.error.$MB_PID
		exit 1
	fi
	roamstat=`grep "+QCFG:" $MOBILE_TMP_DIR/roam.error.$MB_PID |awk -F '\r' '{print $1}'|awk -F ',' '{print $2}'`
	if [ "$roamstat" == "1" ];then
		echo disable > $MOBILE_TMP_DIR/roam.$MB_PID
	elif [ "$roamstat" == "2" ];then
		echo enable > $MOBILE_TMP_DIR/roam.$MB_PID
	elif [ "$roamstat" == "255" ];then
		echo auto > $MOBILE_TMP_DIR/roam.$MB_PID
	else
		echo fail > $MOBILE_TMP_DIR/roam.$MB_PID
	fi
	cp $MOBILE_TMP_DIR/roam.$MB_PID $MOBILE_OUTPUT_DIR/roam_state
	cat $MOBILE_TMP_DIR/roam.$MB_PID >&1

	rm $MOBILE_TMP_DIR/roam.$MB_PID
	rm $MOBILE_TMP_DIR/roam.error.$MB_PID
	
}


set_roamstate(){
	# - Refer to 4.3.3, take effect immediately
	# - New AT command doc 3.3.2, it will always take effect immediately
	#   And no auto setting
	case "$1" in
		"auto")
			$DNICHAT $DEVPORT AT+QCFG=\"roamservice\",2 2> $MOBILE_TMP_DIR/setroam.error
			;;
		"on")
			$DNICHAT $DEVPORT AT+QCFG=\"roamservice\",2 2> $MOBILE_TMP_DIR/setroam.error
			;;
		"off")
			$DNICHAT $DEVPORT AT+QCFG=\"roamservice\",1 2> $MOBILE_TMP_DIR/setroam.error
			;;
		*)
			echo "Error: invalid arg"
			return
			;;
	esac

	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/setroam.error` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/setroam.error >&2
		echo "fail $exit_num" >&1
		rm $MOBILE_TMP_DIR/setroam.error
		exit 1
	fi
	echo "success" >&1
	rm $MOBILE_TMP_DIR/setroam.error
}
set_pin_mode(){
	PINMODE=$2
	NEWPIN=$3
	#get andi verify the pin code is valid?
	valid_pinpuk "pin" "$NEWPIN"
	if [ $? -ne 0 ];then
		echo "new pin code rule is error, you need input 4 - 8 number" >&2
		echo fail >&1
		exit 1
	fi
	$DNICHAT $DEVPORT AT+CLCK=\"SC\",$PINMODE,\"$NEWPIN\" 2> $MOBILE_TMP_DIR/setpinmode.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/setpinmode.error` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/setpinmode.error >&2
		local curmode=`$DNICHAT $DEVPORT AT+CLCK=\"SC\",2 2>&1|grep "+CLCK:"|awk -F '\r' '{print $1}'|awk -F ': ' '{print $2}'`
		comp_pinpuk "pin" "$NEWPIN"
		if [ $? -eq 0 ] && [ "x$curmode" == "x$PINMODE" ];then
			rm $MOBILE_TMP_DIR/setpinmode.error
			echo "success" >&1
			exit 0
		fi
		echo "fail" >&1
		rm $MOBILE_TMP_DIR/setpinmode.error
		exit 1
	fi
	$CONFIG_CMD set wan2.pin_code=$NEWPIN
	if [ $PINMODE == "1" ];then
		$DNICHAT $DEVPORT AT+CFUN=0 2> $MOBILE_TMP_DIR/software-restart.error
		exit_num=$?
		if [ $exit_num -ne 0 ];then
			echo "fail" >&1
			echo "dnichat exit num: $exit_num" >&2
			exit 1
		fi
		sleep 1
		$DNICHAT $DEVPORT AT+CFUN=1 2>> $MOBILE_TMP_DIR/software-restart.error
		exit_num=$?
		if [ $exit_num -ne 0 ];then
			echo "fail" >&1
			echo "dnichat exit num: $exit_num" >&2
			exit 1
		fi
		sleep 1
	fi
	echo "success" >&1
	rm $MOBILE_TMP_DIR/setpinmode.error
}
get_pin_mode(){
	$DNICHAT $DEVPORT AT+CLCK=\"SC\",2 2> $MOBILE_TMP_DIR/pinmode.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/pinmode.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/pinmode.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/pinmode.error.$MB_PID
		exit 1
	fi
	pinmode=`grep "+CLCK:" $MOBILE_TMP_DIR/pinmode.error.$MB_PID |awk -F '\r' '{print $1}'|awk -F ': ' '{print $2}'`
	if [ "$pinmode" == "0" ];then
		echo unlock > $MOBILE_TMP_DIR/pinmode.$MB_PID
	elif [ "$pinmode" == "1" ];then
		echo lock > $MOBILE_TMP_DIR/pinmode.$MB_PID
	fi
	cp $MOBILE_TMP_DIR/pinmode.$MB_PID $MOBILE_OUTPUT_DIR/pin_mode
	cat $MOBILE_TMP_DIR/pinmode.$MB_PID >&1

	rm $MOBILE_TMP_DIR/pinmode.$MB_PID
	rm $MOBILE_TMP_DIR/pinmode.error.$MB_PID
		
}
get_pinpuk_count(){
	get_user_option $@
	$DNICHAT $DEVPORT AT+QPINC? 2> $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID` != "x" ];then
		echo "dnichat exit num: $exit_num" >&2
		grep ERROR $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID
		exit 1
	fi
	grep "+QPINC:" $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID |grep SC |awk -F ',' '{print "pin:"$2"\n""puk:"$3}' > $MOBILE_TMP_DIR/pinpuk_count.$MB_PID
	case $OUTPUT_OPT in 
		get)
		grep $OUTPUT_ITEM $MOBILE_TMP_DIR/pinpuk_count.$MB_PID|awk -F ":" '{print $2}' >&1
		;;
		*)
		cat $MOBILE_TMP_DIR/pinpuk_count.$MB_PID >&1
		;;
	esac
	cp $MOBILE_TMP_DIR/pinpuk_count.$MB_PID $MOBILE_OUTPUT_DIR/pinpuk_count
	rm $MOBILE_TMP_DIR/pinpuk_count.$MB_PID
	rm $MOBILE_TMP_DIR/pinpuk_count.error.$MB_PID
}
#this passwd is related at+clck
#when use it need cpin is ready
change_passwd(){
	OLDPIN=$2
	NEWPIN=$3
	#get and verify the pin code is valid?
	valid_pinpuk "pin" "$NEWPIN"
	if [ $? -ne 0 ];then
		echo "new pin code rule is error, you need input 4 - 8 number" >&2
		echo fail >&1
		exit 1
	fi
	valid_pinpuk "pin" "$OLDPIN"
	if [ $? -ne 0 ];then
		echo "old pin code rule is error, you need input 4 - 8 number" >&2
		echo fail >&1
		exit 1
	fi
	sim_status=`$DNICHAT $DEVPORT AT+CPIN? 2>&1|grep "+CPIN:"|awk -F ": " '{print $2}'|awk -F '\r' '{print $1}'`
	if [ "x$sim_status" == "xSIM PIN" ];then
		$DNICHAT $DEVPORT AT+CPIN=$OLDPIN
		if [ $? -ne 0 ];then
			echo fail >&1
			exit 1
		fi
		sleep 1
	fi
	$DNICHAT $DEVPORT AT+CPWD=\"SC\",\"$OLDPIN\",\"$NEWPIN\" 2> $MOBILE_TMP_DIR/change_passwd.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/change_passwd.error` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/change_passwd.error >&2
		echo "fail" >&1
		rm $MOBILE_TMP_DIR/change_passwd.error
		exit 1
	fi
	$CONFIG_CMD set wan2.pin_code=$NEWPIN
	echo "success" >&1

}
#this cmd is to set mobile network scan mode
set_mbscanmode(){
	case $2 in
		"3G") mode=2;;
		"4G") mode=3;;
		*)    mode=0;;
	esac
	$DNICHAT $DEVPORT AT+QCFG=\"nwscanmode\",$mode,1 2> $MOBILE_TMP_DIR/set-netmode.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/set-netmode.error` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/set-netmode.error >&2
		echo "fail" >&1
		rm $MOBILE_TMP_DIR/set-netmode.error
		exit 1
	fi
	echo "success" >&1

}
#del sms info use flash index 
del_sms(){
	local index=$2
	local mode=$3
	$DNICHAT $DEVPORT AT+CMGD=$index,$mode 2> $MOBILE_TMP_DIR/del-sms.error
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ x`grep ERROR $MOBILE_TMP_DIR/del-sms.error` != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/del-sms.error >&2
		echo "fail" >&1
		rm $MOBILE_TMP_DIR/del-sms.error
		exit 1
	fi
	echo "success" >&1

}
#get sim card own phone number
get_sim_number(){
	$DNICHAT $DEVPORT AT+CNUM 2> $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID`" != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID
		exit 1
	fi
	grep +CNUM: $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID |awk -F ',' '{print $2}'|sed 's:\"::g' > $MOBILE_TMP_DIR/get-sim-number.$MB_PID
	cat $MOBILE_TMP_DIR/get-sim-number.$MB_PID >&1
	cp $MOBILE_TMP_DIR/get-sim-number.$MB_PID $MOBILE_OUTPUT_DIR/sim_number
	rm $MOBILE_TMP_DIR/get-sim-number.error.$MB_PID
	rm $MOBILE_TMP_DIR/get-sim-number.$MB_PID
}

# Quectel EGxx thermal setting
egxx_thermal_on()
{
	echo "EGxx thermal set start."
	# TODO - Thermal level setting

	# Thermal protection on
	$DNICHAT $DEVPORT at+qcfg=\"thermal/limit_rates\",1
	[ $? -eq 0 ] && echo "Pass." || echo "Fail."
	echo "."
}

egxx_b13_disable()
{
	local firmware_region=`cat /tmp/firmware_region | awk '{print $1}'`
	if [ "x$firmware_region" = "xNA" ] ;then
		echo "EGxx disable Band B13"
		$DNICHAT $DEVPORT at+qcfg=\"band\",0,\"42000001003300285a\",0,1
		[ $? -eq 0 ] && echo "Pass." || echo "Fail."
		echo "."
	fi
}

# Init eg18 configure
# Setting saved in LTE module, so only need set one time at boot
lte_init_setting()
{
	# Enable thermal protection
	egxx_thermal_on

	# If NA region disable B13
	egxx_b13_disable

	# Thermal range setting
	set_egxx_otp
}

#get eg18 pa temperature
get_pa_temp(){
	$DNICHAT $DEVPORT AT+QTEMP 2> $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID
	exit_num=$?
	if [ $exit_num -ne 0 ] || [ "x`grep ERROR $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID`" != "x" ];then
		grep ERROR $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID
		exit 1
	fi
	awk -F "\"" '/tsens_tz_sensor1/ {print $4}' $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID > $MOBILE_TMP_DIR/get-pa-temp.$MB_PID
	cat $MOBILE_TMP_DIR/get-pa-temp.$MB_PID >&1
	cp $MOBILE_TMP_DIR/get-pa-temp.$MB_PID $MOBILE_OUTPUT_DIR/pa_temperature
	rm $MOBILE_TMP_DIR/get-pa-temp.error.$MB_PID
	rm $MOBILE_TMP_DIR/get-pa-temp.$MB_PID
}

#set lte OTP function
set_egxx_otp()
{
	echo "EGxx thermal otp set start valid after reboot."
	# Thermal range setting
	# Thermal threshold
	# Level-1:  high threshold 105 C; low threshold 100 C
	# Level-2:  high threshold 110 C; low threshold 105 C


	# Thermal protection thermal range
	$DNICHAT $DEVPORT at+qcfg=\"thermal/modem\",1,105000,100000
	[ $? -eq 0 ] && echo "Pass." || echo "Fail."
	echo "."
	$DNICHAT $DEVPORT at+qcfg=\"thermal/modem\",2,110000,105000
	[ $? -eq 0 ] && echo "Pass." || echo "Fail."
	echo "."
}

mbctrl_init(){
	mkdir $MOBILE_OUTPUT_DIR
	mkdir $MOBILE_TMP_DIR
}

#checkout option
get_user_option(){
	local count=0
	for opt in $@
	do
		let count+=1
		case  $opt in
		get)
			OUTPUT_OPT=$opt
			let count+=1
			[ $count -le $# ]&& OUTPUT_ITEM=`eval echo '$'"$count"`
			let count-=1
			break
		;;
		esac
	
	done
}
get_band_freq(){
        FDL_low="2110 1930 1805 2110 869 875 2620 925 1844.9 2110 1475.9 729 746 758 -1 -1 734 860 875 791 1495.9 3510 2180 1525 1930 859 852 758 717 2350 462.5 1452 1900 2010 1850 1930 1910 2570 1880 2300 2496 3400 3600 703 1447 5150 5855 3550 3550 1432 1427 3300 2483.5 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 2100 2100 738 753 2570 1995 617 461 460 1475 1432 1427 -1 -1 -1 -1 -1 -1 -1 -1 728 -1 420 422"
        NOffs_DL="0 600 1200 1950 2400 2650 2750 3450 3800 4150 4750 5010 5180 5280 -1 -1 5730 5850 6000 6150 6450 6600 7500 7700 8040 8690 9040 9210 9660 9770 9870 9920 36000 36200 36350 36950 37550 37750 38250 38650 39650 41590 43590 45590 46590 46790 54540 55240 56740 58240 59090 59140 60140 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 65536 66436 67336 67536 67836 68336 68586 68926 68986 69036 69466 70316 -1 -1 -1 -1 -1 -1 -1 -1 70366 -1 70546 70596"
        FUL_low="1920 1850 1710 1710 824 830 2500 880 1749.9 1710 1427.9 699 777 788 -1 -1 704 815 830 832 1447.9 3410 2000 1626.5 1850 814 807 703 -1 2305 452.5 -1 1900 2010 1850 1930 1910 2570 1880 2300 2496 3400 3600 703 1447 5150 5855 3550 3550 1432 1427 3300 2483.5 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1920 1710 -1 698 -1 1695 663 451 450 1427 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 698 -1 410 412"
        NOffs_UL="18000 18600 19200 19950 20400 20650 20750 21450 21800 22150 22750 23010 23180 23280 -1 -1 23730 23850 24000 24150 24450 24600 25500 25700 26040 26690 27040 27210 -1 27660 27760 -1 36000 36200 36350 36950 37550 37750 38250 38650 39650 41590 43590 45590 46590 46790 54540 55240 56740 58240 59090 59140 60140 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 131072 131972 -1 132672 -1 132972 133122 133472 133522 133572 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 134002 -1 134182 134232"
        band_name="2100,1900 PCS,1800+,AWS-1,850,UMTS only,2600,900 GSM,1800,AWS-1+,1500 Lower, 700 a,700 c,700 PS, , ,700 b,800 Lower,800 Upper,800 DD,1500 Upper,3500,2000 S-band,1600 L-band,1900+,850+,800 SMR,700 APT,700 d,2300 WCS,450,1500 L-band,TD 1900,TD 2000,TD PCS Lower,TD PCS Upper,TD PCS Center gap,TD 2600,TD 1900+,TD 2300,TD 2600+,TD 3500,TD3700,TD700,TD 1500,TD Unlicensed,TD V2X,TD 3600,TD 3600r,TD 1500+,TD 1500-,TD 3300,TD 2500, , , , , , , , , , , ,2100+,AWS-3,700 EU,700 ME,DL 2500,AWS-4,600,450 PMR/PAMR,450 APAC,L-band,DL 1500+,DL 1500-, , , , , , , , ,700 A+, ,410,410+"
        local i=1
        for value in $FDL_low
        do
                eval FDL_low$i=$value
                i=$(($i+1))
        done
        i=1
        for value in $NOffs_DL
        do
                eval NOffs_DL$i=$value
                i=$(($i+1))
        done
        i=1
        for value in $FUL_low
        do
                eval FUL_low$i=$value
                i=$(($i+1))
        done
        i=1
        for value in $NOffs_UL
        do
                eval NOffs_UL$i=$value
                i=$(($i+1))
        done

        if [ "$#" -lt "3" ];then
        $DNICHAT $DEVPORT AT+QENG=\"servingcell\" 2>/tmp/freq_band.txt
        get_value=$(cat /tmp/freq_band.txt|grep QENG|awk -F',' '{print $4,$9,$10}')
        else
                get_value="$3 $4 $5"
        fi
        i=1
        for freq in $get_value
        do
                eval freq$i=$freq
                i=$(($i+1))
        done

        if [ "$freq3" -ge "1" -a "$freq3" -le "28" ]; then
                earfcnUL=$(($freq2+18000))
        elif [ "$freq3" -ge "30" -a "$freq3" -le "31" ]; then
                earfcnUL=$(($freq2+17890))
        elif [ "$freq3" -ge "33" -a "$freq3" -le "52" ]; then
                earfcnUL=$freq2
        elif [ "$freq3" -ge "65" -a "$freq3" -le "56" ]; then
                earfcnUL=$(($freq2+65536))
        elif [ "$freq3" -eq "68" ]; then
                earfcnUL=$(($freq2+65136))
        elif [ "$freq3" -ge "70" -a "$freq3" -le "74" ]; then
                earfcnUL=$(($freq2+64536))
        else
                earfcnUL=$(($freq2+65636))
	fi

        case "$2" in
                "band_name")
                        echo "$band_name" | awk -v t="${freq3}" -F',' '{ print $t }' 2> $MOBILE_TMP_DIR/band_name.error
                        ;;
                "is_tdd")
                        echo "$freq1" 2> $MOBILE_TMP_DIR/is_tdd.error
                        ;;
                "earfcn")
                        echo "$freq2" 2> $MOBILE_TMP_DIR/arfcn.error
                        ;;
                "band_num")
                        echo "$freq3" 2> $MOBILE_TMP_DIR/freq_band_ind.error
                        ;;
                "uplink_freq")
                        ful_n=$(echo "FUL_low$freq3")
                        ful_N=$(($ful_n))
                        if [ "$ful_N" -eq "-1" ]; then
                                echo "-" 2>$MOBILE_TMP_DIR/uplink_freq.error
                        else
                                noffd_un=$(echo "NOffs_UL$freq3")
                                uplink_freq=$(($ful_n+$(($(($earfcnUL-$noffd_un))/10))))
                                echo "$uplink_freq" 2>$MOBILE_TMP_DIR/uplink_freq.error
                        fi
                        ;;
                "downlink_freq")
                        fdl_n=$(echo "FDL_low$freq3")
                        fdl_N=$(($fdl_n))
                        if [ "$fdl_N" -eq "-1" ]; then
                                echo "-" 2>$MOBILE_TMP_DIR/downlink_freq.error
                        else
                                noffd_dn=$(echo "NOffs_DL$freq3")
                                downlink_freq=$(($fdl_n+$(($(($freq2-$noffd_dn))/10))))
                                echo "$downlink_freq" 2> $MOBILE_TMP_DIR/downlink_freq.error
                        fi
                        ;;
                *)
                        echo "Error: invalid arg"
                        exit -1
                        ;;
        esac
}
# get module is in roaming or not currently
# CGREG is for data CREG is for voice
get_cur_roaming(){
	$DNICHAT $DEVPORT AT+CGREG? 2> $MOBILE_TMP_DIR/reg_status.error.$MB_PID
	if [ $? -ne 0 ];then
		grep ERROR $MOBILE_TMP_DIR/reg_status.error.$MB_PID >&2
		rm $MOBILE_TMP_DIR/reg_status.error.$MB_PID
		exit 1
	fi
	reg_status=`grep +CGREG: $MOBILE_TMP_DIR/reg_status.error.$MB_PID |awk -F '\r' '{print $1}'|awk -F ',' '{print $2}'`
	if [ "x${reg_status}" = "x5" ];then
		echo "on" >&1
	else
		echo "off" >&1
	fi
	rm $MOBILE_TMP_DIR/reg_status.error.$MB_PID

}

[ ! -d $MOBILE_TMP_DIR ] && mbctrl_init

case $1 in
	"--get-revision")
		get_revision 
		;;
	"--get-sim-status")
		get_sim_status 
		;;
	"--pin-verify")
		pin_verify $@
		;;
	"--puk-verify")
		puk_verify $@
		;;
	"--get-network-registration")
		get_network_registration $@
		;;
	"--get-signal-strength")
		get_signal_strength $@
		;;
	"--show-message")
		show_message 
		;;
	"--get-adv-info")
		get_adv_info $@ 
		;;
	"--get-support-operator")
		get_support_operator
		;;
	"--get-current-operator")
		get_current_operator
		;;
	"--set-operator-selection")
		set_operator_selection $@
		;;
	"--eg18-upgrade")
		eg18_upgrade $2 
		;;
	"boot")
		mbctrl_init
		;;
	"--lte-init-setting")
		# LTE module init config setting
		lte_init_setting
		;;
	"power-on")
		lte_pwron
		;;
	"power-off")
		lte_pwroff
		;;
	"--set-hot-swap")
		set_hot_swap 
		;;
	"--hot-swap-status")
		get_hot_swap_status
		;;
	"--get-iccid")
		get_iccid
		;;
	"--software-restart")
		software_restart
		;;
	"--def")
		factory_default
		;;
	"--get-imei")
		get_imei $@
		;;
	"--get-imsi")
		get_imsi
		;;
	"--get-roamstate")
		get_roamstate
		;;
	"--set-roamstate")
		shift
		set_roamstate $@
		;;
	"--get-pin-mode")
		get_pin_mode
		;;
	"--set-pin-mode")
		set_pin_mode $@
		;;
	"--get-pinpuk-count")
		get_pinpuk_count $@
		;;
	"--change-passwd")
		change_passwd $@
		;;
	"--set-mbscanmode")
		set_mbscanmode $@
		;;
	"--del-sms")
		del_sms $@
		;;
	"--get-sim-number")
		get_sim_number
		;;
	"--get-pa-temp")
		get_pa_temp
		;;
	"--get-band-freq")
		get_band_freq $@
		;;
	"--get-current-roaming")
		get_cur_roaming
		;;
	*)
		usage
		;;
esac
