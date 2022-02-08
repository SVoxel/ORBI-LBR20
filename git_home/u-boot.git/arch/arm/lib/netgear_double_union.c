/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <mmc.h>
#include <libfdt.h>
#include <fdtdec.h>
#include <post.h>
#include <logbuff.h>

static int factory_default = 0;
extern ulong timeStart;
extern ulong timeDelta;

extern thand_f *timeHandler;

void start_tftp_recovery_mode()
{
    board_power_led(0);
    printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
    StartTftpServerToRecoveFirmware ();/*Enter recovery mode when press reset button to upgrade mode*/
}

int netgear_double_union(void)
{
	char runcmd[256];

	if(board_reset_button_is_press() && board_wps_button_is_press()) {
		printf("enable wireless nmrp !\n");
		board_wireless_nmrp_led(0);
		run_command("setenv bootargs ${bootargs} nmrp=1", 0);
		while (board_reset_button_is_press() || board_wps_button_is_press())
			udelay(10000);
		return 0;
	}
	int i,j;
	int timelimit = 12;
	int switch_mode_flag = 0;

	NetSetTimeout (CONFIG_SYS_HZ/10,board_reset_default_LedSet);
	for (j = 0;j < timelimit ; j++) {
		if(j >= 5) {
			if(!board_wps_button_is_press()){
				if( (j % 2) ==1){
					printf("Factory Reset Mode\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
					board_factory_led(0);
				}
				else{
					printf("                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
					board_factory_led(1);
				}
				factory_default = 1;
			}
			else{
				if( (j % 2) ==1){
					snprintf(runcmd, sizeof(runcmd), "ledctl b_cyan on");
					run_command(runcmd, 0);
					snprintf(runcmd, sizeof(runcmd), "ledctl b_white off");
					run_command(runcmd, 0);
					printf("switch mode flag\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				}
				else{
					snprintf(runcmd, sizeof(runcmd), "ledctl b_cyan off");
					run_command(runcmd, 0);
					snprintf(runcmd, sizeof(runcmd), "ledctl b_white on");
					run_command(runcmd, 0);
					printf("                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
				}
				timelimit = 1000;
				switch_mode_flag = 1;
			}
		}
		/*each cycle delay 1 s (100 * 10000 us)*/
		for (i = 0;i< 100;i++) {
			//reset button isn't pressed
			if(!board_reset_button_is_press() && !board_wps_button_is_press())
			{
				goto next;
			}
			/*first cycle power on test led*/
			if(!i)
			{
				//board_test_led(0);
			}

			if(j >= 5) {
				if (timeHandler && ((get_timer(0) - timeStart) > timeDelta)) {
					thand_f *x;
					x = timeHandler;
					timeHandler = (thand_f *)0;
					(*x)();
				}
			}
			udelay(10000);
		}
	}
	factory_default = 0;
	start_tftp_recovery_mode();
next:
	if(switch_mode_flag){
		board_reset_current_mode_default();
		board_switch_mode_flag();
	}
	run_command("led_i2c_reset_to_blink", 0);
	board_display_led_by_mode_flag();

	if(factory_default)
		board_reset_default();

	return 0;
}
