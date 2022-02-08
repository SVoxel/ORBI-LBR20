int check_wireless_nmrp(void)
{
	int wireless_nmrp = 0;
	if(board_reset_button_is_press() && board_wps_button_is_press()) {
		printf("enable wireless nmrp !\n");
		wireless_nmrp = 1;
		board_wireless_nmrp_led(0);
		while (board_reset_button_is_press() || board_wps_button_is_press())
			udelay(10000);
	}
		return wireless_nmrp ;
}
