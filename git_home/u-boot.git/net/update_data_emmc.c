#include <common.h>

void update_firmware(ulong addr, int firmware_size)
{
	if (firmware_size <= 0) {
		printf("Incorrect firmware size\n");
		return;
	}
	uchar *src_addr;
	ulong target_addr;
	ulong target_cnt;

	target_addr = IMAGE_BASE_BLOCK;
	target_cnt = firmware_size/0x200 + 1;

	char runcmd[256];

	printf ("mmc erase 0x%lx 0x%lx\n",target_addr, target_cnt);
	snprintf(runcmd, sizeof(runcmd), "mmc erase 0x%lx 0x%lx", target_addr, target_cnt);
	run_command(runcmd, 0);
	CheckNmrpAliveTimerExpire(1);

	printf ("Copy image to Flash... ");

	printf ("mmc write 0x%lx 0x%lx 0x%lx\n", addr, target_addr, target_cnt);
	snprintf(runcmd, sizeof(runcmd), "mmc write 0x%lx 0x%lx 0x%lx", addr, target_addr, target_cnt);
	run_command(runcmd, 0);

#if defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2)
	printf ("boot_partition_set 1\n");
	snprintf(runcmd, sizeof(runcmd), "boot_partition_set 1");
	run_command(runcmd, 0);
#endif
	CheckNmrpAliveTimerExpire(1);
	
	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}

void update_firmware_second(ulong addr, int firmware_size)
{
	if (firmware_size <= 0) {
		printf("Incorrect firmware size\n");
		return;
	}
	uchar *src_addr;
	ulong target_addr;
	ulong target_cnt;

	target_addr = IMAGE_BASE_BLOCK_SECOND_FW;
	target_cnt = firmware_size/0x200 + 1;

	char runcmd[256];

	printf ("mmc erase 0x%lx 0x%lx\n",target_addr, target_cnt);
	snprintf(runcmd, sizeof(runcmd), "mmc erase 0x%lx 0x%lx", target_addr, target_cnt);
	run_command(runcmd, 0);
	CheckNmrpAliveTimerExpire(1);

	printf ("Copy image to Flash... ");

	printf ("mmc write 0x%lx 0x%lx 0x%lx\n", addr, target_addr, target_cnt);
	snprintf(runcmd, sizeof(runcmd), "mmc write 0x%lx 0x%lx 0x%lx", addr, target_addr, target_cnt);
	run_command(runcmd, 0);

#if defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) || \
	defined(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) || \
	defined(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2)
	printf ("boot_partition_set 2\n");
	snprintf(runcmd, sizeof(runcmd), "boot_partition_set 2");
	run_command(runcmd, 0);
#endif
	CheckNmrpAliveTimerExpire(1);
	
	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}
