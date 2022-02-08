#!/bin/sh
#
# Copyright (c) 2015 The Linux Foundation. All rights reserved.
# Copyright (C) 2011 OpenWrt.org

. /lib/ipq806x.sh
. /etc/ath/wifi.conf

enable_smp_affinity_wifi() {
	# irq_wifi0=`grep -E -m1 'ath10k' /proc/interrupts | cut -d ':' -f 1 | tail -n1 | tr -d ' '`
	# irq_wifi1=`grep -E -m2 'ath10k' /proc/interrupts | cut -d ':' -f 1 | tail -n1 | tr -d ' '`
	irq_wifi0=`cat /proc/interrupts | grep 'ath10k' | head -n 1 | cut -d ':' -f 1 | tail -n1 | tr -d ' '`
	irq_wifi1=`cat /proc/interrupts | grep 'ath10k' | head -n 2 | cut -d ':' -f 1 | tail -n1 | tr -d ' '`

	# Enable smp_affinity for ath10k driver
	if [ -n "$irq_wifi0" ]; then
		board=$(ipq806x_board_name)

		case "$board" in
			ap-dk0*)
				echo 4 > /proc/irq/$irq_wifi0/smp_affinity
				[ -n "$irq_wifi1" ] && {
				echo 8 > /proc/irq/$irq_wifi1/smp_affinity
				}
			;;
			ap148*)
				echo 2 > /proc/irq/$irq_wifi0/smp_affinity
			;;
		esac
	else
	# Enable smp_affinity for qca-wifi driver
		board=$(ipq806x_board_name)
		device="$1"
		hwcaps=$(cat /sys/class/net/$device/hwcaps)


		case "${hwcaps}" in
			*11an/ac)
				smp_affinity=2
			;;
			*)
				smp_affinity=1
		esac

		case "$board" in
			ap-dk0*)

			#Use rxchainmask to determine that the project is 4x4 or 2x2.
			if [ $(iwpriv wifi2 get_rxchainmask | awk -F ':' '{ print $2 }'    ) == 15     ]; then
				if [ $device == "$a2_device" ]; then
					smp_affinity=2
					irq_affinity_num=174
				elif [ $device == "$g_device" ]; then
					smp_affinity=4
					irq_affinity_num=200
				elif [ $device == "$a_device" ]; then
					smp_affinity=8
					irq_affinity_num=201
				fi
			else
				if [ $device == "$a2_device" ]; then
					smp_affinity=8
					irq_affinity_num=174
				elif [ $device == "$g_device" ]; then
					smp_affinity=4
					irq_affinity_num=200
				elif [ $device == "$a_device" ]; then
					smp_affinity=2
					irq_affinity_num=201
				fi
			fi

			;;
		esac

		[ -n "$irq_affinity_num" ] && echo $smp_affinity > /proc/irq/$irq_affinity_num/smp_affinity
	fi
}

_enable_smp_affinity_dni(){
	device=$1
	local wifi_topology_file=`cat /etc/ath/wifi.conf  | grep WIFI_TOPOLOGY_FILE | awk -F= '{print $2}'`

	[ -n $SMP_AFFINITY_USB -a -f "/proc/irq/$HW_IRQ_NUM_USB/smp_affinity" ] && echo $SMP_AFFINITY_USB > /proc/irq/$HW_IRQ_NUM_USB/smp_affinity
	[ -n $SMP_AFFINITY_WIFI0 -a -f "/proc/irq/$HW_IRQ_NUM_WIFI0/smp_affinity" ] && echo $SMP_AFFINITY_WIFI0 > /proc/irq/$HW_IRQ_NUM_WIFI0/smp_affinity
	[ -n $SMP_AFFINITY_WIFI1 -a -f "/proc/irq/$HW_IRQ_NUM_WIFI1/smp_affinity" ] && echo $SMP_AFFINITY_WIFI1 > /proc/irq/$HW_IRQ_NUM_WIFI1/smp_affinity
	[ -n $SMP_AFFINITY_WIFI2 -a -f "/proc/irq/$HW_IRQ_NUM_WIFI2/smp_affinity" ] && echo $SMP_AFFINITY_WIFI2 > /proc/irq/$HW_IRQ_NUM_WIFI2/smp_affinity
	[ -n $RPS_WWAN0 -a -f "/sys/class/net/$lte_if_index/queues/rx-0/rps_cpus" ] && echo $RPS_WWAN0 > /sys/class/net/$lte_if_index/queues/rx-0/rps_cpus

	for optype in NORMAL BACKHAUL
	do
	    ifname=`awk -v input_optype=$optype -v input_opmode=AP -v input_wifidev=$device -v output_rule=ifname -f /etc/search-wifi-interfaces.awk $wifi_topology_file | cut -d ' ' -f 1`
	    [ -n "$((RPS_${device}))" -a -f "/sys/class/net/$ifname/queues/rx-0/rps_cpus" ] && echo "$((RPS_${device}))" > /sys/class/net/$ifname/queues/rx-0/rps_cpus
	done
}
