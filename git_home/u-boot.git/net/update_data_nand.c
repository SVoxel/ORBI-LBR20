#include <common.h>
#include <errno.h>
#include <nand.h>
#include <dni_common.h>

/**
 * handle_nand_modify_error:
 *
 * Handle erase or write error occured in a NAND erase block.
 *
 * For now, following method is adopted:
 *
 *     * Read the block again. If error, mark the block as bad and reset
 *       board.
 *
 *     * Optionally, if original data which is supposed to be written into the
 *       block is provided, compare read data with it. If 2 data are
 *       different, mark the block as bad and reset board.
 *
 *     * "mark the block as bad and reset board" above takes effect only when
 *       markbad function is implemented in NAND flash driver. If markbad is
 *       not implemented, nothing happens so that behaviors in old version of
 *       code are preserved.
 *
 * @param nand       NAND device
 * @param offset     offset in flash
 * @param orig_data  buffer containing data before being written.
 *                   pass NULL if you do not want to verify written data.
 * @return           never return if block is being tried to be marked as bad
 */
static void handle_nand_modify_error(nand_info_t *nand, ulong offset,
                                     uchar *orig_data)
{
	int rval;
	size_t read_length = CONFIG_SYS_FLASH_SECTOR_SIZE;
	uchar buffer[CONFIG_SYS_FLASH_SECTOR_SIZE];

	printf("Try to read block 0x%lx ... ", offset);
	rval = nand_read(nand, offset, &read_length, buffer);

	/* ECC-correctable block */
	if (rval == -EUCLEAN) {
		rval = 0;
	}
	printf("%s\n", rval ? "ERROR" : "OK");

	if (rval == 0 && orig_data != NULL) {
		puts("Compare written data with original data ... ");
		rval = memcmp(orig_data, buffer,
		              CONFIG_SYS_FLASH_SECTOR_SIZE);
		printf("%s\n", rval ? "DIFFERENT" : "SAME");
	}

	if (rval && nand->block_markbad != NULL) {
		printf("Marking block 0x%lx as bad block ... ", offset);
		rval = nand->block_markbad(nand, offset);
		printf("%s\n", rval ? "FAILED" : "SUCCESS");

		do_reset(NULL, 0, 0, NULL);
	}
}

void update_data(ulong addr, int data_size, ulong target_addr_begin, size_t target_addr_len, int send_nmrp_alive, int mark_bad_reset)
{
	int offset_num;
	uchar *src_addr;
	ulong target_addr;

	if (data_size <= 1) {
		printf("Incorrect data size\n");
		return;
	}

	target_addr = target_addr_begin;
	for (offset_num = 0;
	     offset_num < (((data_size - 1) / CONFIG_SYS_FLASH_SECTOR_SIZE) + 1);
	     offset_num++) {
#if defined(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4)
		if(offset_num >5 && (offset_num % 10) == 0){
			if ( (offset_num / 10) % 2 == 0){
				run_command("ledctl p_red on", 0);
				run_command("ledctl p_green on", 0);
			}
			else{
				run_command("ledctl p_green off", 0);
				run_command("ledctl p_red off", 0);
			}
		}
#endif
		nand_erase_options_t nand_erase_options;
		size_t write_size;
		int ret = 0;

		/* erase 64K */
		while (nand_block_isbad(&nand_info[0], target_addr)) {
			printf("Skipping erasing bad block at 0x%08lx\n", target_addr);
			target_addr += CONFIG_SYS_FLASH_SECTOR_SIZE;
		}
		if (target_addr >= target_addr_begin + target_addr_len)
			goto bad_nand;

		printf("Erasing: off %x, size %x\n", target_addr, CONFIG_SYS_FLASH_SECTOR_SIZE);
		memset(&nand_erase_options, 0, sizeof(nand_erase_options));
		nand_erase_options.length = CONFIG_SYS_FLASH_SECTOR_SIZE;
		nand_erase_options.quiet = 0;
		nand_erase_options.jffs2 = 1;
		nand_erase_options.scrub = 0;
		nand_erase_options.offset = target_addr;
		ret = nand_erase_opts(&nand_info[0], &nand_erase_options);
		printf("%s\n", ret ? "ERROR" : "OK");

		if (mark_bad_reset && ret) {
			handle_nand_modify_error(
				&nand_info[0], target_addr, NULL);
		}

		src_addr = addr + offset_num * CONFIG_SYS_FLASH_SECTOR_SIZE;

		printf("Writing: from RAM addr %x, to NAND off %x, size %x\n", src_addr, target_addr, CONFIG_SYS_FLASH_SECTOR_SIZE);
		write_size = CONFIG_SYS_FLASH_SECTOR_SIZE;
		ret = nand_write_skip_bad(&nand_info[0], target_addr, &write_size, (u_char *)src_addr, 0);
		printf(" %zu bytes written: %s\n", write_size,
		       ret ? "ERROR" : "OK");

		if (mark_bad_reset && ret) {
			handle_nand_modify_error(
				&nand_info[0], target_addr, src_addr);
		}

		CheckNmrpAliveTimerExpire(send_nmrp_alive);
		target_addr += CONFIG_SYS_FLASH_SECTOR_SIZE;
	}
	return;
bad_nand:
	printf("** FAIL !! too many bad blocks, no enough space for data.\n");
}

void update_firmware(ulong addr, int firmware_size)
{
	if (get_len_incl_bad(&nand_info[0], (loff_t)CONFIG_SYS_IMAGE_ADDR_BEGIN,
	    (size_t)firmware_size) > ((size_t)CONFIG_SYS_IMAGE_LEN +
	                              (size_t)board_image_reserved_length()))
	{
		printf("** FAIL !! too many bad blocks, no enough space for firmware image.\n");
		return;
	}

#if defined(CONFIG_HW29764958P0P128P512P3X3P4X4) || \
    defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4) || \
    defined(CONFIG_HW29764958P0P128P512P4X4P4X4PXDSL)
	run_command("ipq_nand linux", 0);
#endif
	update_data(addr, firmware_size, CONFIG_SYS_IMAGE_ADDR_BEGIN,
	            CONFIG_SYS_IMAGE_LEN +
		    (size_t)board_image_reserved_length(), 1, 1);

#ifdef CONFIG_SYS_IMAGE_BASE_ADDR_SECOND
	char runcmd[256];
	printf ("boot_partition_set 1\n");
	snprintf(runcmd, sizeof(runcmd), "boot_partition_set 1");
	run_command(runcmd, 0);
#endif
#if defined(CONFIG_SYS_IMAGE_EXTENDER_ADDR)
	board_reset_mode_flag();
	board_reset_default();
#endif

	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}

#ifdef CONFIG_SYS_IMAGE_BASE_ADDR_SECOND
void update_firmware_second(ulong addr, int firmware_size)
{
	if (get_len_incl_bad(&nand_info[0], (loff_t)CONFIG_SYS_IMAGE_BASE_ADDR_SECOND,
	    (size_t)firmware_size) > ((size_t)CONFIG_SYS_IMAGE_LEN +
	                              (size_t)board_image_reserved_length()))
	{
		printf("** FAIL !! too many bad blocks, no enough space for firmware image.\n");
		return;
	}

	update_data(addr, firmware_size, CONFIG_SYS_IMAGE_BASE_ADDR_SECOND,
	            CONFIG_SYS_IMAGE_LEN +
		    (size_t)board_image_reserved_length(), 1, 1);

	char runcmd[256];
	printf ("boot_partition_set 2\n");
	snprintf(runcmd, sizeof(runcmd), "boot_partition_set 2");
	run_command(runcmd, 0);

	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}
#endif
#ifdef CONFIG_SYS_IMAGE_EXTENDER_ADDR
void update_firmware_extender(ulong addr, int firmware_size)
{
	if (get_len_incl_bad(&nand_info[0], (loff_t)CONFIG_SYS_IMAGE_BASE_ADDR_SECOND,
	    (size_t)firmware_size) > ((size_t)CONFIG_SYS_IMAGE_LEN +
	                              (size_t)board_image_reserved_length()))
	{
		printf("** FAIL !! too many bad blocks, no enough space for firmware image.\n");
		return;
	}

	update_data(addr, firmware_size, CONFIG_SYS_IMAGE_EXTENDER_ADDR,
	            CONFIG_SYS_IMAGE_EXTENDER_LEN +
		    (size_t)board_image_reserved_length(), 1, 1);

	board_reset_mode_flag();
	board_reset_default();

	if(NmrpState != 0)
		return;
	printf ("Done\nRebooting...\n");

	do_reset(NULL,0,0,NULL);
}
#endif
