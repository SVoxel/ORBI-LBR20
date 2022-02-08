#include <common.h>

void update_firmware(ulong addr, int firmware_size)
{
	if (firmware_size <= 0) {
		printf("Incorrect firmware size\n");
		return;
	}
	int offset_num;
	uchar *src_addr;
	ulong target_addr;

	target_addr = CONFIG_SYS_IMAGE_ADDR_BEGIN;
	for (offset_num = 0;
	     offset_num < ((firmware_size / CONFIG_SYS_FLASH_SECTOR_SIZE) + 1);
	     offset_num++) {

		/* erase 64K */
		flash_sect_erase(CONFIG_SYS_IMAGE_ADDR_BEGIN +
				 offset_num * CONFIG_SYS_FLASH_SECTOR_SIZE,
				 CONFIG_SYS_IMAGE_ADDR_BEGIN +
				 ((offset_num + 1) * CONFIG_SYS_FLASH_SECTOR_SIZE) - 1);

		CheckNmrpAliveTimerExpire(1);
		target_addr += CONFIG_SYS_FLASH_SECTOR_SIZE;
	}
	printf ("Copy image to Flash... ");
	target_addr = CONFIG_SYS_IMAGE_ADDR_BEGIN;
	for (offset_num = 0;
	     offset_num < ((firmware_size / CONFIG_SYS_FLASH_SECTOR_SIZE) + 1);
	     offset_num++) {

		src_addr = addr + offset_num * CONFIG_SYS_FLASH_SECTOR_SIZE;
		flash_write(src_addr, target_addr, CONFIG_SYS_FLASH_SECTOR_SIZE);

		CheckNmrpAliveTimerExpire(1);
		target_addr += CONFIG_SYS_FLASH_SECTOR_SIZE;
	}
	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}
