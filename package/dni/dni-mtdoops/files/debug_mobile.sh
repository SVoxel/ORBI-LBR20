#!/bin/sh

DNICHAT=/usr/sbin/dnichat
DEVPORT=/dev/ttyUSB2

MBCTRL_PATH=/usr/sbin/mbctrl.sh
MOBILE_OUTPUT_DIR=/tmp/mobile_output
SIM_STATUS=sim_status
QUETEL_FW_REVISION=Revision
DATE="date "+%Y-%m-%d_%H:%M:%S""
DVSL="=====" # divisional line

FILE_PATH=/tmp
FILE_NAME=/tmp/debug_mobile_log
FILE_SIZE=5

at_cmd_set="AT+qeng=\"servingcell\"
            AT+cops?
            AT+cgpaddr
            AT+cgdcont?
            AT+qnwinfo
            AT+CLCK=\"SC\",2
	    AT+QSPN
	    AT+CSQ
	    AT+QCFG=\"band\"
	    AT+QNETINFO=\"servingcell\"
	    AT+QENG=\"neighbourcell\"
	    AT+CFUN?
	    AT+CREG?
	    AT+CGREG?
	    AT+QTEMP"

mbctrl_opt_set="--get-network-registration
                --get-signal-strength
                --hot-swap-status
                --get-adv-info         
                --get-current-operator"

debug_info_category(){
    printf "%5s %-20s %5s\n" $DVSL $1 $DVSL >> $FILE_NAME
}

at_cmd_output(){
    $DNICHAT $DEVPORT $1 2>> $FILE_NAME
    sleep 2
}

log_file_replacement(){
    filesize=`du -m $1 | awk '{print $1}'`
    # The maximum of each file is 5MB
    if [ $filesize -gt $FILE_SIZE ]; then
        echo "filesize is greater than 5MB, change to another lte-log file"
        # Once 1 file has reached the maximum(5MB), start write to another file
        [ -f $1 ] && mv $1 $1_bak
	log_fixed_value
    fi
}

#collect fixed value init
log_fixed_value()
{
	debug_info_category "Quectel_FW_revision"                                               
	at_cmd_output "ATI"
	debug_info_category "Board_IMEI"
	at_cmd_output "AT+GSN"
	debug_info_category "SIM_card_IMSI"                                               
	at_cmd_output "AT+CIMI"
	debug_info_category "SIM_card_QICCID"                                               
	at_cmd_output "AT+QCCID"
}

log_fixed_value
while [ 1 ]
do
    debug_info_category $($DATE)
    #we have done the get sim status in sim_detetor 10s a time
    debug_info_category "SIM_status"
    [ -f $MOBILE_OUTPUT_DIR/$SIM_STATUS ] && cat $MOBILE_OUTPUT_DIR/$SIM_STATUS >> $FILE_NAME

    for opt_index in $mbctrl_opt_set
    do
        echo "$DVSL $MBCTRL_PATH $opt_index $DVSL" >> $FILE_NAME
        echo "`$MBCTRL_PATH $opt_index`"  >> $FILE_NAME
        sleep 2
    done
    
    for at_cmd_index in $at_cmd_set     
    do
        debug_info_category "$at_cmd_index"
        at_cmd_output $at_cmd_index
    done
    echo "$DVSL$DVSL$DVSL next loop $DVSL$DVSL$DVSL" >> $FILE_NAME
    log_file_replacement $FILE_NAME
    sleep 30
done
