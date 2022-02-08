#include <common.h>
#include "nmrp.h"
#include <errno.h>
#if defined(CONFIG_QCA_IPQ40XX_SECURE_BOOT)
#include <asm/arch-ipq40xx/scm.h>
#endif

#ifdef CONFIG_HW29765641P0P128P256P2X2P2X2P2X2
int nmrp_in = 0;
#endif

#include <dni_common.h>

/* Current timeout handler */
extern thand_f *timeHandler;

extern int flash_sect_erase (ulong, ulong);

/* Check if Alive-timer expires? */
void CheckNmrpAliveTimerExpire(int send_nmrp_alive)
{
	ulong passed;

	passed = get_timer(NmrpAliveTimerStart);
	if ((passed / CONFIG_SYS_HZ) + NmrpAliveTimerBase > NMRP_TIMEOUT_ACTIVE) {
		printf("Active-timer expires\n");
		if (send_nmrp_alive) NmrpSend();
		NmrpAliveTimerBase = NMRP_TIMEOUT_ACTIVE / 4;
		NmrpAliveTimerStart = get_timer(0);
	} else {
		printf("Alive-timer %u\n", (passed / CONFIG_SYS_HZ) + NmrpAliveTimerBase);
		/* If passed 1/4 NMRP_TIMEOUT_ACTIVE,
		 * add 1/4 NMRP_TIMEOUT_ACTIVE to NmrpAliveTimerBase.
		 * This is for avoiding "passed" overflow.
		 */
		if ((passed / CONFIG_SYS_HZ) >= (NMRP_TIMEOUT_ACTIVE / 4)) {
			NmrpAliveTimerBase += NMRP_TIMEOUT_ACTIVE / 4;
			NmrpAliveTimerStart = get_timer(0);
			printf("NmrpAliveTimerBase %u\n", NmrpAliveTimerBase);
		}
	}
}




void StartTftpServerToRecoveFirmware (void)
{
	NetRunTftpServer = 1;
	ulong addr;
	image_header_t *hdr;
	int file_size;
	char *s;

	/* pre-set load_addr from CONFIG_SYS_LOAD_ADDR */
	load_addr = CONFIG_SYS_LOAD_ADDR;

	/* pre-set load_addr from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	}

tftpstart:
	addr = load_addr;
	file_size = NetLoop(TFTPGET);
	if (file_size < 1)
	{
		printf ("\nFirmware recovering from TFTP server is stopped or failed! :( \n");
		NetRunTftpServer = 0;
		return;
	}

	//  copy Image to flash

	if (NmrpState == STATE_CLOSED)
		return;
	else if ( NmrpState !=0 )
		NmrpState = STATE_CLOSING;
	hdr = (image_header_t *)(addr + HEADER_LEN);
#ifndef CONFIG_FIT
	if (!board_model_id_match_open_source_id() &&
	    !image_match_open_source_fw_id(addr) &&
	    ntohl(hdr->ih_magic) != IH_MAGIC) {
		puts ("Bad Magic Number,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif
#if defined(CONFIG_QCA_IPQ40XX_SECURE_BOOT)
	if (image_pass_secureboot_authentication(addr + HEADER_LEN)) {
		puts ("Image did not pass SecureBoot authentication,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif
#ifdef NETGEAR_BOARD_ID_SUPPORT
	if (!board_match_image_hw_id(addr)) {
		puts ("Board HW ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
	if (!board_model_id_match_open_source_id() &&
	    (!board_match_image_model_id(addr) &&
	     !image_match_open_source_fw_id(addr))) {
		puts ("Board MODEL ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
	if (!board_match_image_model_id(addr)) {
		printf("board model id mismatch with image id, updating board ID\n");
		board_update_image_model_id(addr);
	}
#endif
#if defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4)
	if(board_get_image_prdl(CONFIG_SYS_LOAD_ADDR) != 1) {
		puts("Product_line value mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif

	update_firmware(addr + HEADER_LEN, file_size - HEADER_LEN);
	if (NmrpState == STATE_CLOSING)
	{
		net_set_udp_handler(NmrpHandler);
		NmrpSend();
	}
	/*
	 *  It indicates that tftp server would leave running state when
	 *  this function returns.
	 */
	NetRunTftpServer = 0;
}

int do_fw_recovery (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	StartTftpServerToRecoveFirmware();
	return 0;
}

U_BOOT_CMD(
	fw_recovery,	1,	0,	do_fw_recovery,
	"start tftp server to recovery dni firmware image.",
	"- start tftp server to recovery dni firmware image."
);

#if defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2) || \
	defined(CONFIG_SYS_IMAGE_BASE_ADDR_SECOND)
void StartTftpServerToRecoveFirmware_second (void)
{
	NetRunTftpServer = 1;
	ulong addr;
	image_header_t *hdr;
	int file_size;
	char *s;

	/* pre-set load_addr from CONFIG_SYS_LOAD_ADDR */
	load_addr = CONFIG_SYS_LOAD_ADDR;

	/* pre-set load_addr from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	}

tftpstart:
	addr = load_addr;
	file_size = NetLoop(TFTPGET);
	if (file_size < 1)
	{
		printf ("\nFirmware recovering from TFTP server is stopped or failed! :( \n");
		NetRunTftpServer = 0;
		return;
	}

	//  copy Image to flash

	if (NmrpState == STATE_CLOSED)
		return;
	else if ( NmrpState !=0 )
		NmrpState = STATE_CLOSING;
	hdr = (image_header_t *)(addr + HEADER_LEN);
#ifndef CONFIG_FIT
	if (!board_model_id_match_open_source_id() &&
	    !image_match_open_source_fw_id(addr) &&
	    ntohl(hdr->ih_magic) != IH_MAGIC) {
		puts ("Bad Magic Number,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif
#ifdef NETGEAR_BOARD_ID_SUPPORT
	if (!board_match_image_hw_id(addr)) {
		puts ("Board HW ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
	if (!board_model_id_match_open_source_id() &&
	    (!board_match_image_model_id(addr) &&
	     !image_match_open_source_fw_id(addr))) {
		puts ("Board MODEL ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
	if (!board_match_image_model_id(addr)) {
		printf("board model id mismatch with image id, updating board ID\n");
		board_update_image_model_id(addr);
	}
#endif

	update_firmware_second(addr + HEADER_LEN, file_size - HEADER_LEN);
	if (NmrpState == STATE_CLOSING)
	{
		net_set_udp_handler(NmrpHandler);
		NmrpSend();
	}
	/*
	 *  It indicates that tftp server would leave running state when
	 *  this function returns.
	 */
	NetRunTftpServer = 0;
}

int do_fw_recovery_second (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	StartTftpServerToRecoveFirmware_second();
	return 0;
}

U_BOOT_CMD(
	fw_recovery_second,	1,	0,	do_fw_recovery_second,
	"start tftp server to recovery dni firmware image.",
	"- start tftp server to recovery dni firmware image."
);
#endif

void UpgradeFirmwareFromNmrpServer(void)
{
	NetRunTftpServer = 1;
	ulong addr;
	image_header_t *hdr;
	int file_size;
	char *s;

	/* pre-set load_addr from CONFIG_SYS_LOAD_ADDR */
	load_addr = CONFIG_SYS_LOAD_ADDR;

	/* pre-set load_addr from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	}

	addr = load_addr;
	file_size = NetLoop(TFTPGET);
	if (file_size < 1)
	{
		printf ("\nFirmware recovering from TFTP server is stopped or failed! :( \n");
		NetRunTftpServer = 0;
		return;
	}

	NmrpState = STATE_TFTPUPLOADING;
	net_set_udp_handler(NmrpHandler);
	NmrpSend();

#ifdef CONFIG_HW29765641P0P128P256P2X2P2X2P2X2
	nmrp_in = 1;
#endif

	printf("Ignore Magic number checking when upgrade via NMRP,Magic number is %x!\n", IH_MAGIC);
	//  copy Image to flash
#ifdef NETGEAR_BOARD_ID_SUPPORT
	if (board_match_image_hw_id(addr)) {
#if defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4)
		if (!board_get_image_info_2(addr, "bd3")) {
			puts ("Image bd3 not found ,it is forbidden to be written to flash!!\n");
			goto board_id_2_NMRPfail;
		}
#endif
#if defined(CONFIG_QCA_IPQ40XX_SECURE_BOOT)
		int ret;
		char buf;

		ret = scm_call(SCM_SVC_FUSE, QFPROM_IS_AUTHENTICATE_CMD,
				NULL, 0, &buf, sizeof(char));

		if (ret == 0 && buf == 1) {
			printf(" Board SecureBoot is enabled\n");
			char runcmd[256];

			ulong kernel_size = rootfs_addr(addr + HEADER_LEN) + sizeof(image_header_t) - addr - HEADER_LEN;
			
			sprintf(runcmd, "secure_authenticate 0x17 0x%llx 0x%llx", addr + HEADER_LEN, kernel_size);
			if (run_command(runcmd, 0) != CMD_RET_SUCCESS) {
				puts ("secure image authentication failed, it is forbidden to be written to flash!!\n");
				goto secure_authentication_NMRPfail;
			}
		}
#endif
		update_firmware(addr + HEADER_LEN, file_size - HEADER_LEN);
		board_update_image_model_id(addr);
	}
	else {
		puts ("Board HW ID mismatch,it is forbidden to be written to flash!!\n");
#if defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4)
board_id_2_NMRPfail:
		board_nmrp_red_blink();
#endif
#if defined(CONFIG_QCA_IPQ40XX_SECURE_BOOT)
secure_authentication_NMRPfail:
		board_nmrp_led_fail(0);
#endif
		NmrpFail = 1;
	}
#else
	update_firmware(addr + HEADER_LEN, file_size - HEADER_LEN);
#endif

	/* firmware write to flash done */
	NmrpFwUPOption = 0;
	if (NmrpSTUPOption == 1) {
		NmrpState = STATE_CONFIGING;
	} else {
		NmrpState = STATE_CLOSING;
	}
	net_set_udp_handler(NmrpHandler);
	NmrpSend();
	NetRunTftpServer = 0;
}

void UpgradeStringTableFromNmrpServer(int table_num)
{
	NetRunTftpServer = 1;
	ulong addr;
	image_header_t *hdr;
	int file_size;
	char *s;

	/* pre-set load_addr from CONFIG_SYS_LOAD_ADDR */
	load_addr = CONFIG_SYS_LOAD_ADDR;

	/* pre-set load_addr from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	}

	addr = load_addr;
	memset(addr, 0, CONFIG_SYS_STRING_TABLE_LEN);
	file_size = NetLoop(TFTPGET);
	if (file_size < 1)
	{
		printf ("\nUpdating string table %d from TFTP server \
			is stopped or failed! :( \n", table_num);
		NetRunTftpServer = 0;
		return;
	}

	/* TFTP Uploading done */
	NmrpState = STATE_TFTPUPLOADING;
	net_set_udp_handler(NmrpHandler);
	NmrpSend();

	/* Write String Table to flash */
	board_upgrade_string_table((uchar *)addr, table_num, file_size);

	/* upgrade string table done, check if more files */
	NmrpStringTableUpdateIndex++;
	if (NmrpStringTableUpdateIndex == NmrpStringTableUpdateCount)
		NmrpSTUPOption = 0;
	if (NmrpFwUPOption == 0 && NmrpSTUPOption == 0) {
		workaround_qca8337_gmac_nmrp_hang_action();
		workaround_ipq40xx_gmac_nmrp_hang_action();
		printf("Upgrading all done\n");
		NmrpState = STATE_CLOSING;
		net_set_udp_handler(NmrpHandler);
		NmrpSend();
	} else {
		printf("More files to be upgrading\n");
		workaround_qca8337_gmac_nmrp_hang_action();
		workaround_ipq40xx_gmac_nmrp_hang_action();
		NmrpState = STATE_CONFIGING;
		net_set_udp_handler(NmrpHandler);
		NmrpSend();
	}
	NetRunTftpServer = 0;
}

void ResetTftpServer(void)
{
	timeHandler = 0;
	if(NmrpState != 0)
	{
		NmrpState = STATE_CONFIGING;
		NmrpSend();
	}
	else
	net_set_state(NETLOOP_RESTART);
}
void StartNmrpClient(void)
{
        if( NetLoop(NMRP) < 1)
        {
#ifdef CONFIG_HW29765641P0P128P256P2X2P2X2P2X2
				if(nmrp_in == 1){
					board_nmrp_led_fail(0);
				}
#endif
                printf("\n nmrp server is stopped or failed !\n");
                return;
        }
}
void ResetBootup_usual(void)
{
        timeHandler = 0;
        net_set_state(NETLOOP_SUCCESS);
}

int do_nmrp (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	StartNmrpClient();
	return 0;
}

U_BOOT_CMD(
	nmrp,	1,	0,	do_nmrp,
	"start nmrp mechanism to upgrade firmware-image or string-table.",
	"- start nmrp mechanism to upgrade firmware-image or string-table."
);


#if defined(CONFIG_SYS_IMAGE_EXTENDER_ADDR)
void StartTftpServerToRecoveFirmware_extender (void)
{
	NetRunTftpServer = 1;
	ulong addr;
	image_header_t *hdr;
	int file_size;
	int image_prdl;
	char *s;

	/* pre-set load_addr from CONFIG_SYS_LOAD_ADDR */
	load_addr = CONFIG_SYS_LOAD_ADDR;

	/* pre-set load_addr from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	}

tftpstart:
	addr = load_addr;
	file_size = NetLoop(TFTPGET);
	if (file_size < 1)
	{
		printf ("\nFirmware recovering from TFTP server is stopped or failed! :( \n");
		NetRunTftpServer = 0;
		return;
	}

	//  copy Image to flash

	if (NmrpState == STATE_CLOSED)
		return;
	else if ( NmrpState !=0 )
		NmrpState = STATE_CLOSING;
	hdr = (image_header_t *)(addr + HEADER_LEN);
#ifndef CONFIG_FIT
	if (!board_model_id_match_open_source_id() &&
	    !image_match_open_source_fw_id(addr) &&
	    ntohl(hdr->ih_magic) != IH_MAGIC) {
		puts ("Bad Magic Number,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif
#ifdef NETGEAR_BOARD_ID_SUPPORT
	if (!board_match_image_hw_id(addr)) {
		puts ("Board HW ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
	if (!board_model_id_match_open_source_id() &&
	    (!board_match_image_model_id(addr) &&
	     !image_match_open_source_fw_id(addr))) {
		puts ("Board MODEL ID mismatch,it is forbidden to be written to flash!!\n");
		ResetTftpServer();
		goto tftpstart;
	}
#endif

	image_prdl = board_get_image_prdl(CONFIG_SYS_LOAD_ADDR);
	if(image_prdl == 2)
		update_firmware_extender(addr + HEADER_LEN, file_size - HEADER_LEN);
	else
		printf("Product_line value mismatch,it is forbidden to be written to flash!!\n");
	if (NmrpState == STATE_CLOSING)
	{
		net_set_udp_handler(NmrpHandler);
		NmrpSend();
	}
	/*
	 *  It indicates that tftp server would leave running state when
	 *  this function returns.
	 */
	NetRunTftpServer = 0;
}

int do_fw_recovery_extender (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	StartTftpServerToRecoveFirmware_extender();
	return 0;
}

U_BOOT_CMD(
	fw_recovery_extender,	1,	0,	do_fw_recovery_extender,
	"start tftp server to recovery dni firmware image.",
	"- start tftp server to recovery dni firmware image."
);
#endif
