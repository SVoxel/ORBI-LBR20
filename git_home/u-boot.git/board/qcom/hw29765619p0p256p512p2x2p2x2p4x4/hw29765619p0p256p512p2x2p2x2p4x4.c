/*
 *
 * Copyright (c) 2015, 2016 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <common.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/arch-qcom-common/gpio.h>
#include <asm/errno.h>
#include <asm/arch-ipq40xx/ess/ipq40xx_edma.h>
#include <environment.h>
#include <configs/hw29765619p0p256p512p2x2p2x2p4x4.h>
#include "hw29765619p0p256p512p2x2p2x2p4x4.h"
#include "hw29765619p0p256p512p2x2p2x2p4x4_board_param.h"
#include <asm/arch-ipq40xx/scm.h>
#include <nand.h>
#include <phy.h>
#include <part.h>
#include <mmc.h>
#include "ipq40xx_edma_eth.h"
#ifdef CONFIG_IPQ_NAND
#include <linux/mtd/ipq_nand.h>
#include <asm/arch-qcom-common/nand.h>
#else
#include <asm/arch-qcom-common/qpic_nand.h>
#endif
#include <mtd_node.h>
#include <jffs2/load_kernel.h>
#include <asm/arch-qcom-common/clk.h>
#include <asm/hw29765619p0p256p512p2x2p2x2p4x4/smem.h>

DECLARE_GLOBAL_DATA_PTR;

#define ADSS_AUDIO_RXM_CBCR_REG			0x0770012C
#define ADSS_AUDIO_RXB_CBCR_REG			0x0770010C
#define ADSS_AUDIO_TXB_CBCR_REG			0x0770014C
#define ADSS_AUDIO_SPDIF_CBCR_REG		0x07700154
#define ADSS_AUDIO_SPDIF_DIV2_CBCR_REG		0x0770015C
#define ADSS_AUDIO_TXM_CBCR_REG			0x0770016C
#define ADSS_AUDIO_PCM_CBCR_REG			0x077001AC
#define ADSS_AUDIO_SPDIF_IN_FAST_CBCR_REG	0x077001EC

loff_t board_env_offset;
loff_t board_env_range;
loff_t board_env_size;
extern int nand_env_device;
char *env_name_spec;
extern char *mmc_env_name_spec;
extern char *nand_env_name_spec;
int (*saveenv)(void);
env_t *env_ptr;
extern env_t *mmc_env_ptr;
extern env_t *nand_env_ptr;
extern int nand_env_init(void);
extern int nand_saveenv(void);
extern int qpic_nand_init(struct qpic_nand_init_config *config);
extern void nand_env_relocate_spec(void);
extern int ipq40xx_edma_init(ipq40xx_edma_board_cfg_t *edma_cfg);
extern int ipq40xx_qca8075_phy_init(struct ipq40xx_eth_dev *cfg);
extern int ipq40xx_qca8033_phy_init(struct ipq40xx_eth_dev *cfg);
extern void ipq40xx_register_switch(
	int (*sw_init)(struct ipq40xx_eth_dev *cfg));
extern int mmc_env_init(void);
extern int mmc_saveenv(void);
extern void mmc_env_relocate_spec(void);
extern int fdt_node_set_part_info(void *blob, int parent_offset,
					struct mtd_device *dev);
#ifdef CONFIG_IPQ40XX_SPI
extern int ipq_spi_init(u16 idx);
#endif
#ifdef CONFIG_QCA_MMC
qca_mmc mmc_host;
#endif
int flash_read (char *src, unsigned int addr, unsigned int cnt);

extern int spi_nand_init(void);

/*
 * Don't have this as a '.bss' variable. The '.bss' and '.rel.dyn'
 * sections seem to overlap.
 *
 * $ arm-none-linux-gnueabi-objdump -h u-boot
 * . . .
 *  8 .rel.dyn      00004ba8  40630b0c  40630b0c  00038b0c  2**2
 *                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 *  9 .bss          0000559c  40630b0c  40630b0c  00000000  2**3
 *                  ALLOC
 * . . .
 *
 * board_early_init_f() initializes this variable, resulting in one
 * of the relocation entries present in '.rel.dyn' section getting
 * corrupted. Hence, when relocate_code()'s 'fixrel' executes, it
 * patches a wrong address, which incorrectly modifies some global
 * variable resulting in a crash.
 *
 * Moral of the story: Global variables that are written before
 * relocate_code() gets executed cannot be in '.bss'
 */
board_ipq40xx_params_t *gboard_param = (board_ipq40xx_params_t *)0xbadb0ad;

#define DLOAD_DISABLE 0x1
#define RESERVE_ADDRESS_START 0x87B00000 /*TZAPPS, SMEM and TZ Regions */
#define RESERVE_ADDRESS_SIZE 0x500000

#define SET_MAGIC 0x1
#define CLEAR_MAGIC 0x0
#define SCM_CMD_TZ_CONFIG_HW_FOR_RAM_DUMP_ID 0x9
#define SCM_CMD_TZ_FORCE_DLOAD_ID 0x10
#define BOOT_VERSION	0
#define TZ_VERSION	1
#define RD_FAST_BOOT_CONFIG  0x0005E02C

/*******************************************************
 Function description: Board specific initialization.
 I/P : None
 O/P : integer, 0 - no error.

********************************************************/
static board_ipq40xx_params_t *get_board_param(unsigned int machid)
{
	unsigned int index;

	printf("machid : 0x%0x\n", machid);
	for (index = 0; index < NUM_IPQ40XX_BOARDS; index++) {
		if (machid == board_params[index].machid)
			return &board_params[index];
	}
	BUG_ON(index == NUM_IPQ40XX_BOARDS);
	printf("cdp: Invalid machine id 0x%x\n", machid);
	for (;;);
}

int env_init(void)
{
	int ret;
	qca_smem_flash_info_t sfi;

	smem_get_boot_flash(&sfi.flash_type,
				&sfi.flash_index,
				&sfi.flash_chip_select,
				&sfi.flash_block_size,
				&sfi.flash_density);

	if (sfi.flash_type != SMEM_BOOT_MMC_FLASH) {
		ret = nand_env_init();
#ifdef CONFIG_QCA_MMC
	} else {
		ret = mmc_env_init();
#endif
	}

	return ret;
}

void env_relocate_spec(void)
{
	qca_smem_flash_info_t sfi;

	smem_get_boot_flash(&sfi.flash_type,
				&sfi.flash_index,
				&sfi.flash_chip_select,
				&sfi.flash_block_size,
				&sfi.flash_density);

	if (sfi.flash_type != SMEM_BOOT_MMC_FLASH) {
		nand_env_relocate_spec();
#ifdef CONFIG_QCA_MMC
	} else {
		mmc_env_relocate_spec();
#endif
	}

};

/*
 * The audio block is out of reset by default due to which the
 * audio clock blocks are also turned on. When audio TLMM is
 * enabled in kernel, the clocks will also be available at the
 * pins which causes pop noise during kernel bootup.
 * To avoid this, the clocks are turned off in u-boot.
 */
static void disable_audio_clks(void)
{
	writel(0, ADSS_AUDIO_RXM_CBCR_REG);
	writel(0, ADSS_AUDIO_RXB_CBCR_REG);
	writel(0, ADSS_AUDIO_TXB_CBCR_REG);
	writel(0, ADSS_AUDIO_SPDIF_CBCR_REG);
	writel(0, ADSS_AUDIO_SPDIF_DIV2_CBCR_REG);
	writel(0, ADSS_AUDIO_TXM_CBCR_REG);
	writel(0, ADSS_AUDIO_PCM_CBCR_REG);
	writel(0, ADSS_AUDIO_SPDIF_IN_FAST_CBCR_REG);
}

int board_init(void)
{
	int ret;
	uint32_t start_blocks;
	uint32_t size_blocks;
	qca_smem_flash_info_t *sfi = &qca_smem_flash_info;

	gd->bd->bi_boot_params = QCA_BOOT_PARAMS_ADDR;
	gd->bd->bi_arch_number = smem_get_board_platform_type();
	gboard_param = get_board_param(gd->bd->bi_arch_number);

	ret = smem_get_boot_flash(&sfi->flash_type,
					&sfi->flash_index,
					&sfi->flash_chip_select,
					&sfi->flash_block_size,
					&sfi->flash_density);
	if (ret < 0) {
		printf("cdp: get boot flash failed\n");
		return ret;
	}

	/*
	 * Should be inited, before env_relocate() is called,
	 * since env. offset is obtained from SMEM.
	 */
	if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
		ret = smem_ptable_init();
		if (ret < 0) {
			printf("cdp: SMEM init failed\n");
			return ret;
		}
	}

	if (sfi->flash_type == SMEM_BOOT_NAND_FLASH) {
		nand_env_device = CONFIG_QPIC_NAND_NAND_INFO_IDX;
	} else if (sfi->flash_type == SMEM_BOOT_SPI_FLASH) {
		nand_env_device = CONFIG_IPQ_SPI_NOR_INFO_IDX;
	} else if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
		printf("BUG: unsupported flash type : %d\n", sfi->flash_type);
		BUG();
	}

	if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
		ret = smem_getpart("0:APPSBLENV", &start_blocks, &size_blocks);
		if (ret < 0) {
			printf("cdp: get environment part failed\n");
			return ret;
		}

		board_env_offset = ((loff_t) sfi->flash_block_size) * start_blocks;
		board_env_size = ((loff_t) sfi->flash_block_size) * size_blocks;
	}

	if (sfi->flash_type == SMEM_BOOT_NAND_FLASH) {
		board_env_range = CONFIG_ENV_SIZE_MAX;
		BUG_ON(board_env_size < CONFIG_ENV_SIZE_MAX);
	} else if (sfi->flash_type == SMEM_BOOT_SPI_FLASH) {
		board_env_range = board_env_size;
		BUG_ON(board_env_size > CONFIG_ENV_SIZE_MAX);
#ifdef CONFIG_QCA_MMC
	} else if (sfi->flash_type == SMEM_BOOT_MMC_FLASH) {
		board_env_range = CONFIG_ENV_SIZE_MAX;
#endif
	} else {
		printf("BUG: unsupported flash type : %d\n", sfi->flash_type);
		BUG();
	}

	if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
		saveenv = nand_saveenv;
		env_ptr = nand_env_ptr;
		env_name_spec = nand_env_name_spec;
#ifdef CONFIG_QCA_MMC
	} else {
		saveenv = mmc_saveenv;
		env_ptr = mmc_env_ptr;
		env_name_spec = mmc_env_name_spec;
#endif
	}

	if (gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK04_1_C2)
		disable_audio_clks();

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
       printf("orbi-outdoor uboot V2.3-baudrate-9600 for DNI HW ID: hw29765619p0p256p512p2x2p2x2p4x4 .\n");
       return 0;
}
#endif /* CONFIG_DISPLAY_BOARDINFO */


void qca_get_part_details(void)
{
	int ret, i;
	uint32_t start;         /* block number */
	uint32_t size;          /* no. of blocks */

	qca_smem_flash_info_t *smem = &qca_smem_flash_info;

	struct { char *name; qca_part_entry_t *part; } entries[] = {
		{ "0:HLOS", &smem->hlos },
		{ "rootfs", &smem->rootfs },
	};

	for (i = 0; i < ARRAY_SIZE(entries); i++) {
		ret = smem_getpart(entries[i].name, &start, &size);
		if (ret < 0) {
			qca_part_entry_t *part = entries[i].part;
			debug("cdp: get part failed for %s\n", entries[i].name);
			part->offset = 0xBAD0FF5E;
			part->size = 0xBAD0FF5E;
		} else {
			qca_set_part_entry(entries[i].name, smem, entries[i].part, start, size);
		}
	}

	return;
}

int board_late_init(void)
{
    unsigned int machid;
    unsigned int flash_type;
    qca_smem_flash_info_t *sfi = &qca_smem_flash_info;

    if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
        qca_get_part_details();
    }

    /* get machine type from SMEM and set in env */
    machid = gd->bd->bi_arch_number;
    printf("machid: %x\n", machid);
    flash_type = ((readl(RD_FAST_BOOT_CONFIG) & 0x1E ) >> 1);
    setenv_addr("flash_type", (void *)flash_type);
    printf("flash_type: %d\n", flash_type);
    if (machid != 0) {
        setenv_addr("machid", (void *)machid);
        gd->bd->bi_arch_number = machid;
    }

    return 0;
}

/*
 * This function is called in the very beginning.
 * Retreive the machtype info from SMEM and map the board specific
 * parameters. Shared memory region at Dram address 0x40400000
 * contains the machine id/ board type data polulated by SBL.
 */
int board_early_init_f(void)
{
	/* Retrieve from SMEM */
	gboard_param = get_board_param(smem_get_board_platform_type());
	return 0;
}

void enable_caches(void)
{
	icache_enable();
}

void clear_l2cache_err(void)
{
	return;
}

static void reset_crashdump(void)
{
	unsigned int magic_cookie = CLEAR_MAGIC;
	unsigned int clear_info[] =
		{ 1 /* Disable wdog debug */, 0 /* SDI enable*/, };
	scm_call(SCM_SVC_BOOT, SCM_CMD_TZ_CONFIG_HW_FOR_RAM_DUMP_ID,
		(const void *)&clear_info, sizeof(clear_info), NULL, 0);
	scm_call(SCM_SVC_BOOT, SCM_CMD_TZ_FORCE_DLOAD_ID, &magic_cookie,
			sizeof(magic_cookie), NULL, 0);
}
void reset_cpu(ulong addr)
{
	run_command("led_i2c_reset", 0);
	/* Clear Debug sw entry register */
	reset_crashdump();
	/* clear ps-hold bit to reset the soc */
	writel(0, GCNT_PSHOLD);
	while (1);
}

int dram_init(void)
{
	struct smem_ram_ptable rtable;
	int i;
	int mx = ARRAY_SIZE(rtable.parts);

	if (smem_ram_ptable_init(&rtable) > 0) {
		gd->ram_size = 0;
		for (i = 0; i < mx; i++) {
			if (rtable.parts[i].category == RAM_PARTITION_SDRAM &&
			    rtable.parts[i].type == RAM_PARTITION_SYS_MEMORY) {
				gd->ram_size += rtable.parts[i].size;
			}
		}
		gboard_param->ddr_size = gd->ram_size;
	} else {
		gd->ram_size = gboard_param->ddr_size;
	}
	return 0;
}

void board_nand_init(void)
{
	gpio_func_data_t *gpio;
#ifdef CONFIG_IPQ_NAND
	ipq_nand_init(IPQ_NAND_LAYOUT_LINUX, QCOM_NAND_QPIC);
#else
	if ((gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK04_1_C1) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_DB_DK02_1_C1) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK06_1_C1) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK07_1_C1) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK07_1_C2) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK04_1_C4) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK04_1_C5)) {

		struct qpic_nand_init_config config;
		config.pipes.read_pipe = DATA_PRODUCER_PIPE;
		config.pipes.write_pipe = DATA_CONSUMER_PIPE;
		config.pipes.cmd_pipe = CMD_PIPE;

		config.pipes.read_pipe_grp = DATA_PRODUCER_PIPE_GRP;
		config.pipes.write_pipe_grp = DATA_CONSUMER_PIPE_GRP;
		config.pipes.cmd_pipe_grp = CMD_PIPE_GRP;

		config.bam_base = IPQ40xx_QPIC_BAM_CTRL;
		config.nand_base = IPQ40xx_EBI2ND_BASE;
		config.ee = QPIC_NAND_EE;
		config.max_desc_len = QPIC_NAND_MAX_DESC_LEN;
		gpio = gboard_param->nand_gpio;
		if (gpio) {
			qca_configure_gpio(gpio,
				gboard_param->nand_gpio_count);
		}
		gpio = gboard_param->spi_nor_gpio;
		if (gpio) {
			qca_configure_gpio(gpio,
				gboard_param->spi_nor_gpio_count);
		}
		if (gboard_param->nand_gpio)
			qpic_nand_init(&config);
	}
#endif
	if ((gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK01_1_C2) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK05_1_C1) ||
		(gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK04_1_C5)) {
		spi_nand_init();
	}

	if (gboard_param->machid != MACH_TYPE_IPQ40XX_AP_DK07_1_C2) {
#ifdef CONFIG_IPQ40XX_SPI
		ipq_spi_init(CONFIG_IPQ_SPI_NOR_INFO_IDX);
#endif
	}
}

/*
 * Gets the ethernet address from the ART partition table and return the value
 */
int get_eth_mac_address(uchar *enetaddr, uint no_of_macs)
{
	s32 ret = 0 ;
	u32 start_blocks;
	u32 size_blocks;
	u32 length = (6 * no_of_macs);
	u32 flash_type;
	loff_t art_offset;
	qca_smem_flash_info_t *sfi = &qca_smem_flash_info;
#ifdef CONFIG_QCA_MMC
	block_dev_desc_t *blk_dev;
	disk_partition_t disk_info;
	struct mmc *mmc;
	char mmc_blks[512];
#endif
	if (sfi->flash_type != SMEM_BOOT_MMC_FLASH) {
		if (qca_smem_flash_info.flash_type == SMEM_BOOT_SPI_FLASH)
			flash_type = CONFIG_IPQ_SPI_NOR_INFO_IDX;
		else if (qca_smem_flash_info.flash_type == SMEM_BOOT_NAND_FLASH)
			flash_type = CONFIG_IPQ_NAND_NAND_INFO_IDX;
		else {
			printf("Unknown flash type\n");
			return -EINVAL;
		}

		ret = smem_getpart("0:ART", &start_blocks, &size_blocks);
		if (ret < 0) {
			printf("No ART partition found\n");
			return ret;
		}

#if 0
		/*
		 * ART partition 0th position will contain Mac address.
		 */
		art_offset =
		((loff_t) qca_smem_flash_info.flash_block_size * start_blocks);

		ret = nand_read(&nand_info[flash_type],
				art_offset, &length, enetaddr);
		if (ret < 0)
			printf("ART partition read failed..\n");
#endif
		get_board_data(LAN_MAC_OFFSET, length, enetaddr);
#ifdef CONFIG_QCA_MMC
	} else {
		blk_dev = mmc_get_dev(mmc_host.dev_num);
		ret = find_part_efi(blk_dev, "0:HLOS", &disk_info);
		/*
		 * ART partition 0th position will contain MAC address.
		 * Read 1 block.
		 */
		if (ret > 0) {
			mmc = mmc_host.mmc;
			ret = mmc->block_dev.block_read
				(mmc_host.dev_num, disk_info.start,
						1, mmc_blks);
			memcpy(enetaddr, mmc_blks, length);
                }
		if (ret < 0)
			printf("ART partition read failed..\n");
#endif
	}
	return ret;
}

static void ipq40xx_set_ethmac_addr(void)
{
	int i, ret;
	uchar enetaddr[CONFIG_IPQ_NO_MACS * 6];
	uchar *mac_addr;
	char ethaddr[16] = "ethaddr";
	char mac[64];
	/* Get the MAC address from ART partition */
	ret = get_eth_mac_address(enetaddr, CONFIG_IPQ_NO_MACS);
	for (i = 0; (ret >= 0) && (i < CONFIG_IPQ_NO_MACS); i++) {
		mac_addr = &enetaddr[i * 6];
		if (!is_valid_ether_addr(mac_addr)) {
			printf("eth%d MAC Address from ART is not valid\n", i);
		} else {
			/*
			 * U-Boot uses these to patch the 'local-mac-address'
			 * dts entry for the ethernet entries, which in turn
			 * will be picked up by the HLOS driver
			 */
			sprintf(mac, "%x:%x:%x:%x:%x:%x",
					mac_addr[0], mac_addr[1],
					mac_addr[2], mac_addr[3],
					mac_addr[4], mac_addr[5]);
			setenv(ethaddr, mac);
		}
		sprintf(ethaddr, "eth%daddr", (i + 1));
	}
}

static void ipq40xx_edma_common_init(void)
{
	writel(1, GCC_ESS_BCR);
	mdelay(10);
	writel(0, GCC_ESS_BCR);
	mdelay(100);

	writel(1, GCC_MDIO_AHB_CBCR);
	writel(MDIO_CTRL_0_DIV(0xff) |
		MDIO_CTRL_0_MDC_MODE |
		MDIO_CTRL_0_GPHY(0xa), MDIO_CTRL_0_REG);
}

int board_eth_init(bd_t *bis)
{
	u32 status;
	gpio_func_data_t *gpio;
	ipq40xx_edma_common_init();
	gpio = gboard_param->sw_gpio;
	if (gpio) {
		qca_configure_gpio(gpio, gboard_param->sw_gpio_count);
	}
	switch (gboard_param->machid) {
	case MACH_TYPE_IPQ40XX_AP_DK01_1_S1:
	case MACH_TYPE_IPQ40XX_AP_DK01_1_C2:
	case MACH_TYPE_IPQ40XX_AP_DK05_1_C1:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(62));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	case MACH_TYPE_IPQ40XX_AP_DK01_1_C1:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(59));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	case MACH_TYPE_IPQ40XX_AP_DK04_1_C4:
	case MACH_TYPE_IPQ40XX_AP_DK04_1_C1:
	case MACH_TYPE_IPQ40XX_AP_DK04_1_C3:
	case MACH_TYPE_IPQ40XX_AP_DK04_1_C5:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(41));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	 case MACH_TYPE_IPQ40XX_AP_DK04_1_C2:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(67));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	 case MACH_TYPE_IPQ40XX_AP_DK06_1_C1:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(19));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	case MACH_TYPE_IPQ40XX_AP_DK07_1_C1:
	case MACH_TYPE_IPQ40XX_AP_DK07_1_C2:
		mdelay(1);
		writel(GPIO_OUT, GPIO_IN_OUT_ADDR(41));
		ipq40xx_register_switch(ipq40xx_qca8075_phy_init);
		break;
	case MACH_TYPE_IPQ40XX_DB_DK01_1_C1:
	case MACH_TYPE_IPQ40XX_DB_DK02_1_C1:
		gpio = gboard_param->rgmii_gpio;
		if (gpio) {
			qca_configure_gpio(gpio, gboard_param->rgmii_gpio_count);
		}
		ipq40xx_register_switch(ipq40xx_qca8033_phy_init);
		break;
	default:
		break;
	}
	status = ipq40xx_edma_init(gboard_param->edma_cfg);
	return status;
}

void qca_configure_gpio(gpio_func_data_t *gpio, uint count)
{
	int i;

	for (i = 0; i < count; i++) {
		gpio_tlmm_config(gpio->gpio, gpio->func, gpio->out,
			gpio->pull, gpio->drvstr, gpio->oe,
			gpio->gpio_vm, gpio->gpio_od_en, gpio->gpio_pu_res);
		gpio++;
	}
}

#ifdef CONFIG_OF_BOARD_SETUP
struct flash_node_info {
	const char *compat;	/* compatible string */
	int type;		/* mtd flash type */
	int idx;		/* flash index */
};

int ipq_fdt_fixup_spi_nor_params(void *blob)
{
	int nodeoff, ret;
	qca_smem_flash_info_t sfi;
	uint32_t val;

	/* Get flash parameters from smem */
	smem_get_boot_flash(&sfi.flash_type,
				&sfi.flash_index,
				&sfi.flash_chip_select,
				&sfi.flash_block_size,
				&sfi.flash_density);
	nodeoff = fdt_node_offset_by_compatible(blob, -1, "n25q128a11");

	if (nodeoff < 0) {
		printf("ipq: fdt fixup unable to find compatible node\n");
		return nodeoff;
	}

	val = cpu_to_fdt32(sfi.flash_block_size);
	ret = fdt_setprop(blob, nodeoff, "sector-size",
			&val, sizeof(uint32_t));
	if (ret) {
		printf("%s: unable to set sector size\n", __func__);
		return -1;
	}

	val = cpu_to_fdt32(sfi.flash_density);
	ret = fdt_setprop(blob, nodeoff, "density",
			&val, sizeof(uint32_t));
	if (ret) {
		printf("%s: unable to set density\n", __func__);
		return -1;
	}

	return 0;
}

void ipq_fdt_mem_rsvd_fixup(void *blob)
{
	u32 val[2], dload;
	int nodeoff, ret;
	dload = htonl(DLOAD_DISABLE);
	val[0] = htonl(RESERVE_ADDRESS_START);
	val[1] = htonl(RESERVE_ADDRESS_SIZE);

	/* Reserve only the TZ and SMEM memory region and free the rest */
	nodeoff = fdt_path_offset(blob, "/reserved-memory/rsvd2");
	if (nodeoff < 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}
	ret = fdt_del_node(blob, nodeoff);
	if (ret != 0) {
		debug("ipq: fdt fixup unable to delete node\n");
		return;
	}
	nodeoff = fdt_path_offset(blob, "/reserved-memory/wifi_dump");
	if (nodeoff < 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}
	ret = fdt_del_node(blob, nodeoff);
	if (ret != 0) {
		debug("ipq: fdt fixup unable to delete node\n");
		return;
	}
	nodeoff = fdt_path_offset(blob, "/reserved-memory/rsvd1");
	if (nodeoff < 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}
	ret = fdt_setprop(blob, nodeoff, "reg", val, sizeof(val));
	if (ret != 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}

	/* Set the dload_status to DLOAD_DISABLE */
	nodeoff = fdt_path_offset(blob, "/soc/qca,scm_restart_reason");
	if (nodeoff < 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}
	ret = fdt_setprop(blob, nodeoff, "dload_status", &dload, sizeof(dload));
	if (ret != 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}
	reset_crashdump();
}

void ipq_fdt_fixup_version(void *blob)
{
	int nodeoff, ret;
	char ver[OEM_VERSION_STRING_LENGTH + VERSION_STRING_LENGTH + 1];

	nodeoff = fdt_node_offset_by_compatible(blob, -1, "qcom,ipq40xx");

	if (nodeoff < 0) {
		debug("ipq: fdt fixup unable to find compatible node\n");
		return;
	}

	if (!smem_get_build_version(ver, sizeof(ver), BOOT_VERSION)) {
		debug("BOOT Build Version:  %s\n", ver);
		ret = fdt_setprop(blob, nodeoff, "boot_version",
				ver, strlen(ver));
		if (ret)
			debug("%s: unable to set Boot version\n", __func__);
	}

	if (!smem_get_build_version(ver, sizeof(ver), TZ_VERSION)) {
		debug("TZ Build Version:  %s\n", ver);
		ret = fdt_setprop(blob, nodeoff, "tz_version",
				ver, strlen(ver));
		if (ret)
			debug("%s: unable to set TZ version\n", __func__);
	}
}

void ipq_fdt_fixup_mtdparts(void *blob, struct flash_node_info *ni)
{
	struct mtd_device *dev;
	char *parts;
	int noff;

	parts = getenv("mtdparts");
	if (!parts)
		return;

	if (mtdparts_init() != 0)
		return;

	for (; ni->compat; ni++) {
		noff = fdt_node_offset_by_compatible(blob, -1, ni->compat);
		while (noff != -FDT_ERR_NOTFOUND) {
			dev = device_find(ni->type, ni->idx);
			if (dev) {
				if (fdt_node_set_part_info(blob, noff, dev))
					return; /* return on error */
			}

			/* Jump to next flash node */
			noff = fdt_node_offset_by_compatible(blob, noff,
							     ni->compat);
		}
	}
}

struct vlan_tag {
	unsigned int r0;
	unsigned int r1;
};

struct eth_param{
	int nodeoff;
	int mdio_addr;
	int poll;
	int speed;
	int duplex;
	unsigned long gmac_no;
};

static void ipq40xx_set_setprop(void *blob, int nodeoff, unsigned long gmac_no,
							char *str, int val)
{
	int ret;

	ret = fdt_setprop(blob, nodeoff, str, &val, sizeof(val));
	if (ret)
		debug("unable to set property %s for %lu with error %d\n",
							str, gmac_no, ret);
}

static void ipq40xx_populate_eth_params(void *blob, struct eth_param *port)
{
	ipq40xx_set_setprop(blob, port->nodeoff, port->gmac_no,
				"qcom,phy_mdio_addr", htonl(port->mdio_addr));
	ipq40xx_set_setprop(blob, port->nodeoff, port->gmac_no,
				"qcom,poll_required", htonl(port->poll));
	ipq40xx_set_setprop(blob, port->nodeoff, port->gmac_no,
				"qcom,forced_speed", htonl(port->speed));
	ipq40xx_set_setprop(blob, port->nodeoff, port->gmac_no,
				"qcom,forced_duplex", htonl(port->duplex));
}

/*
 * Logic to patch Ethernet params.
 */
static int ipq40xx_patch_eth_params(void *blob, unsigned long gmac_no)
{
	int nodeoff, nodeoff_c;
	int ret, i;
	struct vlan_tag vlan;
	struct eth_param port_config;
	const char *eth2_prop[] = {"/soc/edma/gmac2", "/soc/edma/gmac3",
							"/soc/edma/gmac4"};
	const char *alias_prop[] = {"ethernet2", "ethernet3", "ethernet4"};
	const char *gmac_node[] = {"gmac2", "gmac3", "gmac4"};

	nodeoff = fdt_path_offset(blob, "/aliases");
	if (nodeoff < 0) {
		printf("ipq: fdt fixup unable to find compatible node\n");
		return -1;
	} else {
		debug("Node Found\n");
	}

	for (i = 0; i < (gmac_no - 2); i++) {
		ret = fdt_setprop(blob, nodeoff, alias_prop[i],
			eth2_prop[i], (strlen(eth2_prop[i]) + 1));
		if (ret)
			debug("%s: unable to patch alias\n", __func__);
		nodeoff_c = fdt_path_offset(blob, "/soc/edma");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}

		ret = fdt_add_subnode(blob, nodeoff_c, gmac_node[i]);
		if (ret < 0)
			debug("%s: unable to add node\n", __func__);
	}

	switch (gmac_no) {
	case 3:
		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac1");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x1);
		vlan.r1 = htonl(0x10);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac2");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x3);
		vlan.r1 = htonl(0xE);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);
		break;
	case 4:
		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac1");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x1);
		vlan.r1 = htonl(0x10);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		port_config.nodeoff = nodeoff_c;
		port_config.mdio_addr = 3;
		port_config.poll = 1;
		port_config.speed = 1000;
		port_config.duplex = 1;
		port_config.gmac_no = gmac_no;
		ipq40xx_populate_eth_params(blob, &port_config);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac2");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x3);
		vlan.r1 = htonl(0x8);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		port_config.nodeoff = nodeoff_c;
		port_config.mdio_addr = 2;
		port_config.poll = 1;
		port_config.speed = 1000;
		port_config.duplex = 1;
		port_config.gmac_no = gmac_no;
		ipq40xx_populate_eth_params(blob, &port_config);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac3");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x4);
		vlan.r1 = htonl(0x6);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);
		break;
	case 5:
		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac1");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x1);
		vlan.r1 = htonl(0x10);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac2");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x3);
		vlan.r1 = htonl(0x8);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac3");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x4);
		vlan.r1 = htonl(0x4);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		port_config.nodeoff = nodeoff_c;
		port_config.mdio_addr = 1;
		port_config.poll = 1;
		port_config.speed = 1000;
		port_config.duplex = 1;
		port_config.gmac_no = gmac_no;
		ipq40xx_populate_eth_params(blob, &port_config);

		nodeoff_c = fdt_path_offset(blob, "/soc/edma/gmac4");
		if (nodeoff_c < 0) {
			printf("ipq: unable to find compatiable edma node\n");
			return -1;
		}
		vlan.r0 = htonl(0x5);
		vlan.r1 = htonl(0x2);
		ret = fdt_setprop(blob, nodeoff_c, "vlan_tag",
			&vlan, sizeof(vlan));
		if (ret)
			debug("%s: unable to set property\n", __func__);

		port_config.nodeoff = nodeoff_c;
		port_config.mdio_addr = 0;
		port_config.poll = 1;
		port_config.speed = 1000;
		port_config.duplex = 1;
		port_config.gmac_no = gmac_no;
		ipq40xx_populate_eth_params(blob, &port_config);

		break;
	}
	nodeoff = fdt_node_offset_by_compatible(blob,
			-1, "qcom,ess-edma");
	if (nodeoff < 0) {
		printf("ipq: unable to find compatible edma node\n");
		return -1;
	}

	gmac_no = htonl(gmac_no);
	ret = fdt_setprop(blob, nodeoff, "qcom,num_gmac",
		&gmac_no, sizeof(gmac_no));
	if (ret)
		debug("%s: unable to set property\n", __func__);
	return 0;
}

/*
 * For newer kernel that boot with device tree (3.14+), all of memory is
 * described in the /memory node, including areas that the kernel should not be
 * touching.
 *
 * By default, u-boot will walk the dram bank info and populate the /memory
 * node; here, overwrite this behavior so we describe all of memory instead.
 */
void ft_board_setup(void *blob, bd_t *bd)
{
	u64 memory_start = CONFIG_SYS_SDRAM_BASE;
	u64 memory_size = gboard_param->ddr_size;
	unsigned long gmac_no;
	char *s;
	char *mtdparts = NULL;
	char parts_str[256];
	qca_smem_flash_info_t *sfi = &qca_smem_flash_info;
	struct flash_node_info nodes[] = {
		{ "qcom,msm-nand", MTD_DEV_TYPE_NAND, 0 },
		{ "spinand,mt29f", MTD_DEV_TYPE_NAND, 1 },
		{ "n25q128a11", MTD_DEV_TYPE_NAND, 2 },
		{ NULL, 0, -1 },	/* Terminator */
	};

	fdt_fixup_memory_banks(blob, &memory_start, &memory_size, 1);
	ipq_fdt_fixup_version(blob);
#ifndef CONFIG_QCA_APPSBL_DLOAD
	ipq_fdt_mem_rsvd_fixup(blob);
#endif
	if (sfi->flash_type == SMEM_BOOT_NAND_FLASH) {
		mtdparts = "mtdparts=nand0";
	} else if (sfi->flash_type == SMEM_BOOT_SPI_FLASH) {
		/* Patch NOR block size and density for
		 * generic probe case */
		ipq_fdt_fixup_spi_nor_params(blob);
		if (gboard_param->spi_nand_available &&
			get_which_flash_param("rootfs") == 0) {
			sprintf(parts_str,
				"mtdparts=nand1:0x%x@0(rootfs);spi0.0",
				IPQ_NAND_ROOTFS_SIZE);
			mtdparts = parts_str;
		} else if (gboard_param->nor_nand_available &&
			get_which_flash_param("rootfs") == 0) {
			sprintf(parts_str,
				"mtdparts=nand0:0x%x@0(rootfs);spi0.0",
				IPQ_NAND_ROOTFS_SIZE);
			mtdparts = parts_str;

		} else {
			mtdparts = "mtdparts=spi0.0";
		}
	}


	if (mtdparts) {
		mtdparts = qca_smem_part_to_mtdparts(mtdparts);
		if (mtdparts != NULL) {
			debug("mtdparts = %s\n", mtdparts);
			setenv("mtdparts", mtdparts);
		}
		setenv("mtdids", gboard_param->mtdids);

		ipq_fdt_fixup_mtdparts(blob, nodes);
	}
	s = (getenv("gmacnumber"));
	if (s) {
		strict_strtoul(s, 16, &gmac_no);
		if (gmac_no > 2 && gmac_no < 6)
			ipq40xx_patch_eth_params(blob, gmac_no);
	}
	dcache_disable();
	ipq40xx_set_ethmac_addr();
	fdt_fixup_ethernet(blob);

#ifdef CONFIG_QCA_MMC
        board_mmc_deinit();
#endif

}

#endif /* CONFIG_OF_BOARD_SETUP */

#ifdef CONFIG_QCA_MMC
int board_mmc_env_init(void)
{
	block_dev_desc_t *blk_dev;
	disk_partition_t disk_info;
	int ret;

	if (mmc_init(mmc_host.mmc)) {
		/* The HS mode command(cmd6) is getting timed out. So mmc card is
		 * not getting initialized properly. Since the env partition is not
		 * visible, the env default values are writing into the default
		 * partition (start of the mmc device). So do a reset again.
		 */
		if (mmc_init(mmc_host.mmc)) {
			printf("MMC init failed \n");
			return -1;
		}
	}
	blk_dev = mmc_get_dev(mmc_host.dev_num);
	ret = find_part_efi(blk_dev, "0:APPSBLENV", &disk_info);

	if (ret > 0) {
		board_env_offset = disk_info.start * disk_info.blksz;
		board_env_size = disk_info.size * disk_info.blksz;
		board_env_range = board_env_size;
		BUG_ON(board_env_size > CONFIG_ENV_SIZE_MAX);
	}
	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int ret;
	qca_smem_flash_info_t *sfi = &qca_smem_flash_info;

	if (!gboard_param->mmc_gpio_count)
		return 0;

	qca_configure_gpio(gboard_param->mmc_gpio,
			gboard_param->mmc_gpio_count);

	mmc_host.base = MSM_SDC1_BASE;
	mmc_host.clk_mode = MMC_IDENTIFY_MODE;
	emmc_clock_config(mmc_host.clk_mode);

	ret = qca_mmc_init(bis, &mmc_host);

	if (!ret && sfi->flash_type == SMEM_BOOT_MMC_FLASH) {
		ret = board_mmc_env_init();
	}

	return ret;
}

void board_mmc_deinit(void)
{
	emmc_clock_disable();
}
#endif

#ifdef CONFIG_IPQ40XX_PCI
void pcie_config_gpio(pcie_params_t *cfg, int enable)
{
	int i;
	gpio_func_data_t *gpio_data;
	gpio_data = cfg->pci_gpio;

	for (i = 0; i < cfg->pci_gpio_count; i++) {
		if (enable)
			gpio_tlmm_config(gpio_data->gpio, gpio_data->func,
					gpio_data->out, gpio_data->pull,
					gpio_data->drvstr, gpio_data->oe,
					gpio_data->gpio_vm, gpio_data->gpio_od_en,
					gpio_data->gpio_pu_res);
		else
			gpio_tlmm_config(gpio_data->gpio, gpio_data->func,
					GPIO_OUT_LOW, GPIO_NO_PULL,
					GPIO_2MA, GPIO_OE_DISABLE,
					GPIO_VM_DISABLE, GPIO_OD_DISABLE,
					gpio_data->gpio_pu_res);
		gpio_data++;
	}
}

void pcie_controller_reset(int id)
{
	uint32_t val;
	pcie_params_t *cfg;
	cfg = &gboard_param->pcie_cfg[id];

	/* Enable PCIE CLKS */
	pcie_clock_enable(GCC_PCIE_SLEEP_CBCR);
	pcie_clock_enable(GCC_PCIE_AXI_M_CBCR);
	pcie_clock_enable(GCC_PCIE_AXI_S_CBCR);
	pcie_clock_enable(GCC_PCIE_AHB_CBCR);

	/* Assert cc_pcie20_mstr_axi_ares */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_AXI_M_ARES;
	writel(val, cfg->pcie_rst);

	/* Assert cc_pcie20_slv_axi_ares */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_AXI_S_ARES;
	writel(val, cfg->pcie_rst);

	/* Assert cc_pcie20_core_ares */
	writel(PCIE_RST_CTRL_PIPE_ARES, cfg->pcie_rst);

	/* Assert cc_pcie20_core_sticky_area */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_PIPE_STICKY_ARES;
	writel(val, cfg->pcie_rst);

	/* Assert cc_pcie20_phy_ahb_ares */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_PIPE_PHY_AHB_ARES;
	writel(val, cfg->pcie_rst);

	/* Assert cc_pcie20_mstr_sticky_ares */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_AXI_M_STICKY_ARES;
	writel(val, cfg->pcie_rst);

	gpio_set_value(PCIE_RST_GPIO, GPIO_OUT_LOW);

	/* Assert cc_pcie20_ahb_ares;  */
	val = readl(cfg->pcie_rst);
	val |= PCIE_RST_CTRL_AHB_ARES;
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_ahb_ares */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_PIPE_PHY_AHB_ARES);
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_pciephy_phy_ares*/
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_PIPE_ARES);
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_core_sticky_ares */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_PIPE_STICKY_ARES);
	writel(val, cfg->pcie_rst);

	mdelay(5);

	gpio_set_value(PCIE_RST_GPIO, GPIO_OUT_HIGH);

	/* DeAssert cc_pcie20_mstr_axi_ares */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_AXI_M_ARES);
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_mstr_axi_ares */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_AXI_M_STICKY_ARES);
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_slv_axi_ares */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_AXI_S_ARES);
	writel(val, cfg->pcie_rst);

	/* DeAssert cc_pcie20_phy_ahb_ares  */
	val = readl(cfg->pcie_rst);
	val &= ~(PCIE_RST_CTRL_AHB_ARES);
	writel(val, cfg->pcie_rst);
}

static void ipq_pcie_config_controller(int id)
{
	pcie_params_t 	*cfg;
	cfg = &gboard_param->pcie_cfg[id];

	/*
	 * program and enable address translation region 0 (device config
	 * address space); region type config;
	 * axi config address range to device config address range
	 */
	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_VIEWPORT);

	writel(4, cfg->pcie20 + PCIE20_PLR_IATU_CTRL1);
	writel((1 << 31), cfg->pcie20 + PCIE20_PLR_IATU_CTRL2);
	writel(cfg->axi_conf , cfg->pcie20 + PCIE20_PLR_IATU_LBAR);
	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_UBAR);
	writel((cfg->axi_conf + PCIE_AXI_CONF_SIZE - 1),
				cfg->pcie20 + PCIE20_PLR_IATU_LAR);
	writel(MSM_PCIE_DEV_CFG_ADDR,
				cfg->pcie20 + PCIE20_PLR_IATU_LTAR);
	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_UTAR);

	/*
	 * program and enable address translation region 2 (device resource
	 * address space); region type memory;
	 * axi device bar address range to device bar address range
	 */
	writel(2, cfg->pcie20 + PCIE20_PLR_IATU_VIEWPORT);

	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_CTRL1);
	writel((1 << 31), cfg->pcie20 + PCIE20_PLR_IATU_CTRL2);
	writel(cfg->axi_bar_start, cfg->pcie20 + PCIE20_PLR_IATU_LBAR);
	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_UBAR);
	writel((cfg->axi_bar_start + cfg->axi_bar_size
		- PCIE_AXI_CONF_SIZE - 1), cfg->pcie20 + PCIE20_PLR_IATU_LAR);
	writel(cfg->axi_bar_start, cfg->pcie20 + PCIE20_PLR_IATU_LTAR);
	writel(0, cfg->pcie20 + PCIE20_PLR_IATU_UTAR);

	/* 1K PCIE buffer setting */
	writel(0x3, cfg->pcie20 + PCIE20_AXI_MSTR_RESP_COMP_CTRL0);
	writel(0x1, cfg->pcie20 + PCIE20_AXI_MSTR_RESP_COMP_CTRL1);
}

void pcie_linkup(int id)
{
	int j, val;
	pcie_params_t 		*cfg;
	cfg = &gboard_param->pcie_cfg[id];

	pcie_clock_enable(GCC_PCIE_SLEEP_CBCR);
	pcie_clock_enable(GCC_PCIE_AXI_M_CBCR);
	pcie_clock_enable(GCC_PCIE_AXI_S_CBCR);
	pcie_clock_enable(GCC_PCIE_AHB_CBCR);

	pcie_controller_reset(id);
	mdelay(200);

	writel(SLV_ADDR_SPACE_SZ, cfg->parf + PARF_SLV_ADDR_SPACE_SIZE);
	mdelay(100);

	writel(0x0, cfg->pcie20 + PCIE_0_PORT_FORCE_REG);
	val = (L1_ENTRANCE_LATENCY(3) |
		L0_ENTRANCE_LATENCY(3) |
		COMMON_CLK_N_FTS(128) |
		ACK_N_FTS(128));
	writel(val, cfg->pcie20 + PCIE_0_ACK_F_ASPM_CTRL_REG);

	val = (FAST_TRAINING_SEQ(128) |
		NUM_OF_LANES(2) |
		DIRECT_SPEED_CHANGE);
	writel(val, cfg->pcie20 + PCIE_0_GEN2_CTRL_REG);
	writel(PCI_TYPE0_BUS_MASTER_EN,
		cfg->pcie20 + PCIE_0_TYPE0_STATUS_COMMAND_REG_1);

	writel(DBI_RO_WR_EN, cfg->pcie20 + PCIE_0_MISC_CONTROL_1_REG);
	writel(0x0002FD7F, cfg->pcie20 + 0x84);

	val = (PCIE_CAP_ASPM_OPT_COMPLIANCE |
		PCIE_CAP_LINK_BW_NOT_CAP |
		PCIE_CAP_DLL_ACTIVE_REP_CAP |
		PCIE_CAP_L1_EXIT_LATENCY(4) |
		PCIE_CAP_L0S_EXIT_LATENCY(4) |
		PCIE_CAP_MAX_LINK_WIDTH(1) |
		PCIE_CAP_MAX_LINK_SPEED(1));
	writel(val, cfg->pcie20 + PCIE_0_LINK_CAPABILITIES_REG);

	writel(PCIE_CAP_CPL_TIMEOUT_DISABLE,
		cfg->pcie20 + PCIE_0_DEVICE_CONTROL2_DEVICE_STATUS2_REG);

	writel(0x10110008, cfg->pcie20 + PCIE_0_TYPE0_LINK_CONTROL_LINK_STATUS_REG_1);

	writel(LTSSM_EN, cfg->parf + PCIE_0_PCIE20_PARF_LTSSM);

	mdelay(200);

	for (j = 0; j < 400; j++) {
		val = readl(cfg->pcie20 + PCIE_0_TYPE0_LINK_CONTROL_LINK_STATUS_REG_1);
		if (val & (1 << 29)) {
			printf("PCI%d Link Intialized\n", id);
			cfg->linkup = 1;
			break;
		}
		udelay(100);
	}
	ipq_pcie_config_controller(id);
}

void board_pci_init()
{
	int i;
	pcie_params_t *cfg;

	if (gboard_param->machid == MACH_TYPE_IPQ40XX_AP_DK07_1_C2)
		return;

	for (i = 0; i < PCI_MAX_DEVICES; i++) {
		cfg = &gboard_param->pcie_cfg[i];
		pcie_config_gpio(cfg, ENABLE);

		pcie_controller_reset(i);

		pcie_linkup(i);
	}
}

void board_pci_deinit(void)
{
	int i;
	pcie_params_t 		*cfg;

	for (i = 0; i < PCI_MAX_DEVICES; i++) {
		cfg = &gboard_param->pcie_cfg[i];

		writel(1, cfg->parf + PCIE20_PARF_PHY_CTRL);

		pcie_config_gpio(cfg, DISABLE);
	}

	/* Disable PCIE CLKS */
	pcie_clock_disable(GCC_PCIE_SLEEP_CBCR);
	pcie_clock_disable(GCC_PCIE_AXI_M_CBCR);
	pcie_clock_disable(GCC_PCIE_AXI_S_CBCR);
	pcie_clock_disable(GCC_PCIE_AHB_CBCR);
}
#endif /* CONFIG_IPQ40XX_PCI */

/*
 * Routine to get GPIO INPUT pin or set GPIO OUTPUT pin.
 *
 * Modified from gmac_handle_gpio() of drivers/net/nss/mii_gpio.c
 *
 * @param is_get     1: get INPUT pin, 0: set OUTPUT pin
 * @param pin        GPIO pin to get/set
 * @param write_val  value to write in set operation
 * @return           pin value in get; 0 in set
 */
#define GPIO_IN   0
#define GPIO_OUT  1
#define GPIO_SET  0
#define GPIO_GET  1
static inline uint32_t ipq40xx_handle_gpio(uint32_t is_get, uint32_t pin,
                                           uint32_t write_val)
{
    uint32_t addr = GPIO_IN_OUT_ADDR(pin);
    uint32_t val = readl(addr);

    if (is_get)
        return val & (1<<GPIO_IN);

    val &= (~(1 << GPIO_OUT));
    if (write_val)
        val |= (1 << GPIO_OUT);
    writel(val, addr);

    return 0;
}


int misc_init_r(void)
{
	gpio_tlmm_config(RESET_BUTTON, 0, GPIO_OUT_HIGH, GPIO_PULL_UP, LED_CURRENT, GPIO_OE_DISABLE, 0, 0, 0);
	gpio_tlmm_config(WPS_BUTTON, 0, GPIO_OUT_HIGH, GPIO_PULL_UP, LED_CURRENT, GPIO_OE_DISABLE, 0, 0, 0);

	gpio_tlmm_config(POWER_LED_GREEN, 0, GPIO_OUT_LOW, GPIO_PULL_UP, LED_CURRENT, GPIO_OE_ENABLE, 0, 0, 0);
	gpio_tlmm_config(POWER_LED_RED, 0, GPIO_OUT_LOW, GPIO_PULL_UP, LED_CURRENT, GPIO_OE_ENABLE, 0, 0, 0);

	run_command("i2c probe", 0);
	run_command("led_i2c_reset", 0);

        char buf[BOARD_BOOT_PARTITION_LENGTH + 1] = {0};
        int val = 1;
        char runcmd[256];

        get_board_data_2(BOARD_MODE_FLAG_OFFSET, BOARD_MODE_FLAG_LENGTH, buf);
        val = (int)simple_strtol(buf, NULL, 16);

	run_command("ledctl p_green on", 0);
	if(val ==2)
		run_command("ledctl b_cyan on", 0);
	else
		run_command("ledctl b_white on", 0);
	ipq40xx_handle_gpio(GPIO_SET, HEATER, 0);
	return 0;
}


/*ledstat 0:on; 1:off*/
void board_power_led(int ledstat)
{
	if (ledstat == 0){
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 1);
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 0);
	} else {
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 0);
	}
}

/*ledstat 0:on; 1:off*/
void board_test_led(int ledstat)
{
	if (ledstat == 0){
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 1);
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 0);
    } else {
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 0);
    }
}

void board_factory_led(int ledstat)
{
// blink between red and amber
       if (ledstat == 0){
               ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 1);

               ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 1);
    } else {
               ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 0);
    }
}

/*ledstat 0:on; 1:off*/
void board_wireless_nmrp_led(int ledstat)
{
	run_command("led_i2c_reset", 0);
	if (ledstat == 0)
		run_command("ledctl b_amber on", 0);
}

int do_led_i2c_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ipq40xx_handle_gpio(GPIO_SET, LED_GREEN, 0);
	ipq40xx_handle_gpio(GPIO_SET, LED_RED, 0);
	ipq40xx_handle_gpio(GPIO_SET, LED_BLUE, 0);
	ipq40xx_handle_gpio(GPIO_SET, LED_WHITE, 0);
	ipq40xx_handle_gpio(GPIO_SET, LED_WHITE_TOP, 0);

	run_command("i2c mw 0x27 0x0 0x01;i2c mw 0x27 0x1 0x03;"
				"i2c mw 0x27 0x2 0xff;i2c mw 0x27 0x3 0xff;i2c mw 0x27 0x4 0xff;i2c mw 0x27 0x5 0xff;"
				"i2c mw 0x27 0x6 0xff;i2c mw 0x27 0x7 0xff;i2c mw 0x27 0x8 0xff;i2c mw 0x27 0x9 0xff;"
				"i2c mw 0x27 0xa 0xff;i2c mw 0x27 0xb 0x00;"
				"i2c mw 0x27 0xc 0x66;i2c mw 0x27 0xd 0x66;"
				"i2c mw 0x27 0xe 0x92;i2c mw 0x27 0xf 0x94;i2c mw 0x27 0x10 0x98;i2c mw 0x27 0x11 0xd0;", 0);

	run_command("i2c mw 0x23 0x0 0x01;i2c mw 0x23 0x1 0x03;"
				"i2c mw 0x23 0x2 0xff;i2c mw 0x23 0x3 0xff;i2c mw 0x23 0x4 0xff;i2c mw 0x23 0x5 0xff;"
				"i2c mw 0x23 0x6 0xff;i2c mw 0x23 0x7 0xff;i2c mw 0x23 0x8 0xff;i2c mw 0x23 0x9 0xff;"
				"i2c mw 0x23 0xa 0xff;i2c mw 0x23 0xb 0x00;"
				"i2c mw 0x23 0xc 0x66;i2c mw 0x23 0xd 0x66;"
				"i2c mw 0x23 0xe 0x92;i2c mw 0x23 0xf 0x94;i2c mw 0x23 0x10 0x98;i2c mw 0x23 0x11 0xd0;", 0);

	return 0;
}

U_BOOT_CMD(
       led_i2c_reset, 1, 0, do_led_i2c_reset,
       "Reset all leds, please use ledctl to 'light' led.",
       "\n"
);

int do_led_i2c_reset_to_blink(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	run_command("led_i2c_reset", 0);

	run_command("i2c mw 0x27 0x1 0x23;"
				"i2c mw 0x27 0xa 0x7f;i2c mw 0x27 0xb 0x3b;"
				"i2c mw 0x27 0xc 0xff;i2c mw 0x27 0xd 0xff;", 0);

	run_command("i2c mw 0x23 0x1 0x23;"
				"i2c mw 0x23 0xa 0x7f;i2c mw 0x23 0xb 0x3b;"
				"i2c mw 0x23 0xc 0xff;i2c mw 0x23 0xd 0xff;", 0);

	return 0;
}

U_BOOT_CMD(
       led_i2c_reset_to_blink, 1, 0, do_led_i2c_reset_to_blink,
       "Reset all leds, please use ledctl to 'blink' led.",
       "\n"
);

int do_ledctl (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int gpio_pin, on_off;
	int gpio_pin2 = 0;

	if (argc != 3)
		goto error;

	if (strcmp(argv[1], "p_green") == 0)
		gpio_pin = POWER_LED_GREEN;
	else if (strcmp(argv[1], "p_red") == 0)
		gpio_pin = POWER_LED_RED;
	else if (strcmp(argv[1], "t_white") == 0)
		gpio_pin = LED_WHITE_TOP;
	else if (strcmp(argv[1], "b_red") == 0)
		gpio_pin = LED_RED;
	else if (strcmp(argv[1], "b_green") == 0)
		gpio_pin = LED_GREEN;
	else if (strcmp(argv[1], "b_blue") == 0)
		gpio_pin = LED_BLUE;
	else if (strcmp(argv[1], "b_white") == 0)
		gpio_pin = LED_WHITE;
	else if (strcmp(argv[1], "b_amber") == 0) {
		gpio_pin = LED_RED;
		gpio_pin2 = LED_GREEN;
	} else if (strcmp(argv[1], "b_cyan") == 0) {
		gpio_pin = LED_GREEN;
		gpio_pin2 = LED_BLUE;
	} else if (strcmp(argv[1], "b_magenta") == 0) {
		gpio_pin = LED_BLUE;
		gpio_pin2 = LED_RED;
	}else if ( !strcmp(argv[1], "b_all") && !strcmp(argv[2], "off")){
		ipq40xx_handle_gpio(GPIO_SET, LED_GREEN, 0);
		ipq40xx_handle_gpio(GPIO_SET, LED_BLUE, 0);
		ipq40xx_handle_gpio(GPIO_SET, LED_RED, 0);
		ipq40xx_handle_gpio(GPIO_SET, LED_WHITE, 0);
		return 0;
	}
	else
		goto error;

	if (strcmp(argv[2], "on") == 0)
		on_off = 1;
	else if (strcmp(argv[2], "off") == 0)
		on_off = 0;
	else
		goto error;

	ipq40xx_handle_gpio(GPIO_SET, gpio_pin, on_off);
	if (gpio_pin2)
		ipq40xx_handle_gpio(GPIO_SET, gpio_pin2, on_off);

	return 0;

error:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	ledctl, 3, 0, do_ledctl,
		"ledctl - on/off LEDs.",
		"ledctl p_green on/off   : turn led_power_green on/off\n"
		"ledctl p_red on/off     : turn led_power_red on/off\n"
		"ledctl t_white on/off   : turn led_top_white on/off\n"
		"ledctl b_red on/off     : turn led_bottom_red on/off\n"
		"ledctl b_green on/off   : turn led_bottom_green on/off\n"
		"ledctl b_blue on/off    : turn led_bottom_blue on/off\n"
		"ledctl b_white on/off   : turn led_bottom_white on/off\n"
		"ledctl b_amber on/off   : turn led_bottom amber (red + green) on/off\n"
		"ledctl b_cyan on/off    : turn led_bottom cyan (green + blue) on/off\n"
		"ledctl b_magenta on/off : turn led_bottom magenta (blue + red) on/off\n"
		"ledctl b_all off : turn off all led \n"
);

/*erase the config sector of flash*/
void board_reset_default(void)
{
    nand_erase_options_t nand_erase_options;
    int ret = 0;

    printf("Restore to factory default\n");
    memset(&nand_erase_options, 0, sizeof(nand_erase_options));
    nand_erase_options.length = CONFIG_SYS_FLASH_CONFIG_PARTITION_SIZE;
    nand_erase_options.quiet = 0;
    nand_erase_options.jffs2 = 1;
    nand_erase_options.scrub = 0;
    nand_erase_options.offset = CONFIG_SYS_FLASH_CONFIG_BASE;
    ret = nand_erase_opts(&nand_info[0], &nand_erase_options);
    printf("nand erase base config %s\n", ret ? "ERROR" : "OK");
    nand_erase_options.length = CONFIG_SYS_FLASH_CONFIG_EXTENDER_PARTITION_SIZE;
    nand_erase_options.offset = CONFIG_SYS_FLASH_CONFIG_EXTENDER;
    ret = nand_erase_opts(&nand_info[0], &nand_erase_options);
    printf("nand erase extender config %s\n", ret ? "ERROR" : "OK");

    board_reset_mode_flag();
    if(NmrpState != 0)
        return;
    printf("Rebooting...\n");
    do_reset(NULL,0,0,NULL);
}

int board_reset_mode_flag(void){
	char buf[BOARD_MODE_FLAG_LENGTH + 1] = {0};
	char runcmd[256];
	int val ;

	get_board_data_2(BOARD_DEFAULT_MODE_FLAG_OFFSET,BOARD_DEFAULT_MODE_FLAG_LENGTH,buf);
	val = (int)simple_strtol(buf, NULL, 16);
	printf("copy default mode flag %d to mode flag\n",val);
	if(val == 2)
		snprintf(runcmd, sizeof(runcmd), "mode_flag_set 2");
	else
		snprintf(runcmd, sizeof(runcmd), "mode_flag_set 1");

	run_command(runcmd, 0);
}

void board_reset_current_mode_default()
{
	char buf[BOARD_BOOT_PARTITION_LENGTH + 1] = {0};
	int val ;
	char runcmd[256];
	nand_erase_options_t nand_erase_options;
	int ret = 0;

	get_board_data_2(BOARD_MODE_FLAG_OFFSET, BOARD_MODE_FLAG_LENGTH, buf);
	val = (int)simple_strtol(buf, NULL, 16);
	printf("mode_flag : %d\n", val);
	memset(&nand_erase_options, 0, sizeof(nand_erase_options));
	if(val == 2){
		nand_erase_options.length = CONFIG_SYS_FLASH_CONFIG_EXTENDER_PARTITION_SIZE;
		nand_erase_options.offset = CONFIG_SYS_FLASH_CONFIG_EXTENDER;
		printf("Restore extender to factory default\n");
	}
	else{
		nand_erase_options.length = CONFIG_SYS_FLASH_CONFIG_PARTITION_SIZE;
		nand_erase_options.offset = CONFIG_SYS_FLASH_CONFIG_BASE;
		printf("Restore base to factory default\n");
	}

	nand_erase_options.quiet = 0;
	nand_erase_options.jffs2 = 1;
	nand_erase_options.scrub = 0;
	ret = nand_erase_opts(&nand_info[0], &nand_erase_options);
	printf("nand erase config %s\n", ret ? "ERROR" : "OK");
}

void board_reset_default_LedSet(void)
{
  static int DiagnosLedCount = 1;
	if ((DiagnosLedCount++ % 2) == 1) {
		/*power on test led 0.25s */
		//board_test_led(0);
		NetSetTimeout((CONFIG_SYS_HZ* 1) / 4, board_reset_default_LedSet);
	} else {
		/*power off test led 0.75s */
		//board_test_led(1);
		NetSetTimeout((CONFIG_SYS_HZ * 3) / 4, board_reset_default_LedSet);
	}
}

void board_firmware_corrupted_LedSet(void)
{
	ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 0);
	int i;
	for(i=0;i<=4;i++){
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 1);
		mdelay(250);
		ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 0);
		mdelay(750);
	}
	ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 1);
}

/*return value 0: not pressed, 1: pressed*/
int board_reset_button_is_press(void)
{
	if (ipq40xx_handle_gpio(GPIO_GET, RESET_BUTTON, 0))
		return 0;
	else
		return 1;
}

/*return value 0: not pressed, 1: pressed*/
int board_wps_button_is_press(void)
{
	if (ipq40xx_handle_gpio(GPIO_GET, WPS_BUTTON, 0))
		return 0;
	else
		return 1;
}

int do_button_test (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
       if(!board_reset_button_is_press())
               printf("RESET button: NOT pressed\n");
       else
               printf("RESET button: Pressed\n");

       if(!board_wps_button_is_press())
               printf("WPS button: NOT pressed\n");
       else
               printf("WPS button: Pressed\n");

       return 0;
}

U_BOOT_CMD(
       button_test,1 , 1, do_button_test,
       "Test buttons",
       "- Test buttons\n"
       "Press and hold button to be tested before issuing this command.\n"
);

#if defined(NETGEAR_BOARD_ID_SUPPORT)
/*
 * item_name_want could be "device" to get Model Id, "version" to get Version
 * or "hd_id" to get Hardware ID.
 */
void board_get_image_info(ulong fw_image_addr, char *item_name_want, char *buf)
{
    char image_header[HEADER_LEN+1];
    char item_name[HEADER_LEN+1];
    char *item_value;
    char *parsing_string;

    memset(image_header, 0, HEADER_LEN);
    memcpy(image_header, fw_image_addr, HEADER_LEN);
    image_header[HEADER_LEN]='\0';

    parsing_string = strtok(image_header, "\n");
    while (parsing_string != NULL) {
       char *colon_p;
       memset(item_name, 0, sizeof(item_name));
       colon_p = strchr(parsing_string, ':');
       if (colon_p == NULL) {
           break;
       }
       strncpy(item_name, parsing_string, (int)(colon_p - parsing_string));

       if (strcmp(item_name, item_name_want) == 0) {
           item_value = strstr(parsing_string, ":") + 1;

           memcpy(buf, item_value, strlen(item_value));
       }

       parsing_string = strtok(NULL, "\n");
    }
}

int board_match_image_hw_id (ulong fw_image_addr)
{
    char board_hw_id[BOARD_HW_ID_LENGTH + 1];
    char image_hw_id[BOARD_HW_ID_LENGTH + 1];

    /*get hardward id from board */
    memset(board_hw_id, 0, sizeof(board_hw_id));
    get_board_data(BOARD_HW_ID_OFFSET, BOARD_HW_ID_LENGTH, (u8 *)board_hw_id);
    printf("HW ID on board: %s\n", board_hw_id);

    /*get hardward id from image */
    memset(image_hw_id, 0, sizeof(image_hw_id));
    board_get_image_info(fw_image_addr, "hd_id", image_hw_id);
    printf("HW ID on image: %s\n", image_hw_id);

    /* Only check first 21 chars 29765285+16+0+256+2x2 */
    if (memcmp(board_hw_id, image_hw_id, 21) != 0) {
            printf("Firmware Image HW ID do not match Board HW ID\n");
            return 0;
    }
    printf("Firmware Image HW ID matched Board HW ID\n\n");
    return 1;
}

int board_match_image_model_id (ulong fw_image_addr)
{
    char board_model_id[BOARD_MODEL_ID_LENGTH + 1];
    char image_model_id[BOARD_MODEL_ID_LENGTH + 1];

    /*get hardward id from board */
    memset(board_model_id, 0, sizeof(board_model_id));
    get_board_data(BOARD_MODEL_ID_OFFSET, BOARD_MODEL_ID_LENGTH, (u8 *)board_model_id);
    printf("MODEL ID on board: %s\n", board_model_id);

    /*get hardward id from image */
    memset(image_model_id, 0, sizeof(image_model_id));
    board_get_image_info(fw_image_addr, "device", image_model_id);
    printf("MODEL ID on image: %s\n", image_model_id);

    if (memcmp(board_model_id, image_model_id, 6) != 0) {
            printf("Firmware Image MODEL ID do not match Board model ID\n");
            return 0;
    }
    printf("Firmware Image MODEL ID matched Board model ID\n\n");
    return 1;
}

void board_update_image_model_id (ulong fw_image_addr)
{
}
#if defined(OPEN_SOURCE_ROUTER_SUPPORT) && defined(OPEN_SOURCE_ROUTER_ID)
int board_model_id_match_open_source_id(void)
{
	char *s;

	s = getenv("modelid");
	if (!s) goto empty_out;

	return strcmp(s, OPEN_SOURCE_ROUTER_ID) == 0;

empty_out:
	return 0;
}

int board_image_reserved_length(void)
{
	return board_model_id_match_open_source_id() * CONFIG_RESERVED_LEN;
}
#endif
#endif	/* BOARD_ID */

void board_upgrade_string_table(unsigned char *load_addr, int table_number, unsigned int file_size)
{
    unsigned char *string_table_addr, *addr2;
    unsigned long offset;
    unsigned int table_length;
    unsigned char high_bit, low_bit;
    unsigned long passed;
    int offset_num;
    uchar *src_addr;
    ulong target_addr;
    size_t total_len;
    int ret = 0;

	total_len = CONFIG_SYS_STRING_TABLE_LEN * CONFIG_SYS_STRING_TABLE_NUMBER;
	total_len = ((total_len - 1) / CONFIG_SYS_FLASH_SECTOR_SIZE + 1 ) * CONFIG_SYS_FLASH_SECTOR_SIZE;

    string_table_addr = load_addr + CONFIG_SYS_STRING_TABLE_LEN;

    /*
     * Read whole string table partition from Flash to
     * RAM(load_addr + CONFIG_SYS_STRING_TABLE_LEN) which is just after new
     * string table sent by NMRP server.
     */
    memset(string_table_addr, 0, total_len);
    ret = nand_read_skip_bad(
            &nand_info[0], CONFIG_SYS_STRING_TABLE_ADDR_BEGIN, &total_len,
            (u_char *)string_table_addr);
    printf(" %zu bytes read: %s\n", total_len,
	       ret ? "ERROR" : "OK");

    /* Save string table checksum to (CONFIG_SYS_STRING_TABLE_LEN - 1) */
    memcpy(load_addr + CONFIG_SYS_STRING_TABLE_LEN - 1, load_addr + file_size- 1, 1);

    /* Remove checksum from string table file's tail */
    memset(load_addr + file_size - 1, 0, 1);

    table_length = file_size - 1;
    printf("string table length is %d\n", table_length);

    /* Save (string table length / 1024) to (CONFIG_SYS_STRING_TABLE_LEN - 4) */
    high_bit = table_length / 1024;
    addr2 = load_addr + CONFIG_SYS_STRING_TABLE_LEN - 4;
    memcpy(addr2, &high_bit, sizeof(high_bit));

	/* Save ((string table length % 1024) / 256) to (CONFIG_SYS_STRING_TABLE_LEN - 3) */
    high_bit = (table_length % 1024) / 256;
    addr2 = load_addr + CONFIG_SYS_STRING_TABLE_LEN - 3;
    memcpy(addr2, &high_bit, sizeof(high_bit));

    /* Save ((string table length % 1024) % 256) to (CONFIG_SYS_STRING_TABLE_LEN - 2) */
    low_bit = (table_length % 1024) % 256;
    addr2 = load_addr + CONFIG_SYS_STRING_TABLE_LEN - 2;
    memcpy(addr2, &low_bit, sizeof(low_bit));

    /* Copy processed string table from load_addr to RAM */
    offset = (table_number - 1) * CONFIG_SYS_STRING_TABLE_LEN;
    memcpy(string_table_addr + offset, load_addr, CONFIG_SYS_STRING_TABLE_LEN);

    /* Write back string tables from RAM to Flash; replace update_date(). */
    update_data(string_table_addr, total_len,
                CONFIG_SYS_STRING_TABLE_ADDR_BEGIN,
                CONFIG_SYS_STRING_TABLE_TOTAL_LEN, 1, 1);
    return;
}

int board_flash_read (char *src, ulong addr, ulong cnt)
{
	char cmdbuf[256];

	snprintf(cmdbuf, sizeof(cmdbuf), "nand read 0x%lX 0x%lX 0x%lX", (ulong)src, addr, cnt);
	debug("cmd : %s\n", cmdbuf);
	if (run_command(cmdbuf, 0) != CMD_RET_SUCCESS) {
		printf ("%s failed : %s\n", __FUNCTION__, cmdbuf);
		return 1;
	}
	return 0;
}

int board_flash_sect_erase (ulong addr_first, ulong addr_last)
{
	char cmdbuf[256];

	snprintf(cmdbuf, sizeof(cmdbuf), "nand erase 0x%lX 0x%lX", addr_first, (addr_last - addr_first + 1));
	printf("cmd : %s\n", cmdbuf);
	if (run_command(cmdbuf, 0) != CMD_RET_SUCCESS) {
		printf ("%s failed : %s\n", __FUNCTION__, cmdbuf);
		return 1;
	}
	return 0;
}

int board_flash_write (char *src, ulong addr, ulong cnt)
{
	char cmdbuf[256];

	snprintf(cmdbuf, sizeof(cmdbuf), "nand write 0x%lX 0x%lX 0x%lX", (ulong)src, addr, cnt);
	printf("cmd : %s\n", cmdbuf);
	if (run_command(cmdbuf, 0) != CMD_RET_SUCCESS) {
		printf ("%s failed : %s\n", __FUNCTION__, cmdbuf);
		return 1;
	}
	return 0;
}

int board_flash_enough (int data_size, ulong off, ulong max_len, char* data_name)
{
	int dev = nand_curr_device;
	nand_info_t * nand = &nand_info[dev];
	ulong block_size = nand->erasesize;
	ulong len = 0;
	ulong len_incl_bad = 0;

	while (len < data_size) {
		if (off >= nand->size)
			goto not_enough;

		if (!nand_block_isbad (nand, (loff_t) off))
			len += block_size;

		len_incl_bad += block_size;
		off += block_size;
	}

	if (max_len >= len_incl_bad)
		return 1;

not_enough:
	if (data_name)
		printf("** FAIL !! no enough space for %s.\n", data_name);
	return 0;
}

int board_flash_sect_isbad (ulong off)
{
	int dev = nand_curr_device;
	nand_info_t * nand = &nand_info[dev];

	if (nand_block_isbad(nand, (loff_t) off)) {
		printf("Device %d offset 0x%lX is bad.\n", dev, off);
		return 1;
	}
	return 0;
}

static ulong rootfs_addr(ulong load_addr)
{
	void* hdr;
	ulong image_size, size;

	hdr = (void*) load_addr;
#ifdef CONFIG_FIT
	/* fdt_header->totalsize is the image 'total' size */
	size = ntohl(((struct fdt_header*) hdr) ->totalsize);
	if ((size < 0) || (size > CONFIG_SYS_IMAGE_LEN))
		goto error;
	image_size = size + sizeof(image_header_t);
#else
	/* image_header_t->ih_size is the image 'data' size */
	size = ntohl(((image_header_t*) hdr) ->ih_size);
	if ((size < 0) || (size > CONFIG_SYS_IMAGE_LEN))
		goto error;
	image_size = sizeof(image_header_t) + size + sizeof(image_header_t);
#endif
	/* dniimg rule : "kernel_image + padding + rootfs_image_header" must align to flash section */
	image_size = ((image_size+CONFIG_SYS_FLASH_SECTOR_SIZE-1)/CONFIG_SYS_FLASH_SECTOR_SIZE)*CONFIG_SYS_FLASH_SECTOR_SIZE;

	return (load_addr + image_size - sizeof(image_header_t));

error:
	printf ("kernel imge header corrupted !\n");
	return 0;
}

#define CMDFMT_chk_kernel_rootfs(CMDBUF, ADDR_KERNEL, ADDR_ROOTFS, CMD_OK, CMD_ERROR) \
	snprintf(CMDBUF, sizeof(CMDBUF), \
		"if iminfo 0x%lX; then "\
			"echo kernel checksum OK !;"\
			"if iminfo 0x%lX; then "\
				"echo rootfs checksum OK !;"CMD_OK";true;"\
			"else "\
				"echo rootfs checksum error !!!;"CMD_ERROR";false;"\
			"fi;"\
		"else "\
			"echo kernel checksum error !!!;"CMD_ERROR";false;"\
		"fi;", ADDR_KERNEL, ADDR_ROOTFS)

static ulong dniimg_len(ulong flash_addr)
{
	ulong load_addr = CONFIG_SYS_LOAD_ADDR;
	ulong kernel_size, rootfs_size, dniimg_size;
	void* hdr;

	board_flash_read((char*)load_addr, flash_addr, CONFIG_SYS_FLASH_SECTOR_SIZE);
	if (rootfs_addr(load_addr) == 0)
		goto error;

	kernel_size = rootfs_addr(load_addr) + sizeof(image_header_t) - load_addr;
#ifdef DNI_NAND
	board_flash_read((char*)load_addr, flash_addr, kernel_size);
	hdr = (void*) rootfs_addr(load_addr);
#else
	board_flash_read((char*)load_addr, (flash_addr+kernel_size-CONFIG_SYS_FLASH_SECTOR_SIZE), CONFIG_SYS_FLASH_SECTOR_SIZE);
	hdr = (void*) (load_addr + CONFIG_SYS_FLASH_SECTOR_SIZE - sizeof(image_header_t));
#endif
	rootfs_size = ntohl(((image_header_t*) hdr) ->ih_size);
	if ((rootfs_size < 0) || (rootfs_size > CONFIG_SYS_IMAGE_LEN))
		goto error1;

	dniimg_size = kernel_size + rootfs_size;
	if (dniimg_size > CONFIG_SYS_IMAGE_LEN)
		goto error;

	printf ("dniimg_len is %d ", (int)dniimg_size);
	dniimg_size = ((dniimg_size+CONFIG_SYS_FLASH_SECTOR_SIZE-1)/CONFIG_SYS_FLASH_SECTOR_SIZE)*CONFIG_SYS_FLASH_SECTOR_SIZE;
	printf ("(aligned to %d)\n", (int)dniimg_size);

	return dniimg_size;

error1:
	printf ("rootfs imge header corrupted !\n");
error:
	return 0;
}

static int _load_chk_dniimg(ulong load_addr, char image, char last_try)
{
	ulong img_size;
	char cmdbuf[512];

	printf("Loading firmware %d ...\n", image);

	img_size = (image == 1) ? dniimg_len(CONFIG_SYS_IMAGE_BASE_ADDR) : dniimg_len(CONFIG_SYS_IMAGE_BASE_ADDR_SECOND);
	if (img_size == 0) {
		if (last_try)
			run_command("setenv checkfail 1 ;fw_recovery", 0);
		return -1;
	}

	board_flash_read((char*)load_addr,
	                 (image == 1) ? CONFIG_SYS_IMAGE_BASE_ADDR : CONFIG_SYS_IMAGE_BASE_ADDR_SECOND, img_size);

	if (last_try) {
		if (image == 1)
			CMDFMT_chk_kernel_rootfs(cmdbuf, load_addr, rootfs_addr(load_addr), "boot_partition_set 1", "setenv checkfail 1;fw_recovery");
		else
			CMDFMT_chk_kernel_rootfs(cmdbuf, load_addr, rootfs_addr(load_addr), "boot_partition_set 2", "setenv checkfail 1;fw_recovery");
	}
	else
		CMDFMT_chk_kernel_rootfs(cmdbuf, load_addr, rootfs_addr(load_addr), "", "");

	printf("%s", cmdbuf);
	return (run_command(cmdbuf, 0) == CMD_RET_SUCCESS) ? 0 : -1;
}

static int load_chk_dniimg_extender(ulong load_addr)
{
	ulong img_size;
	char cmdbuf[512];

	printf("Loading extender firmware %d ...\n");

	img_size = dniimg_len(CONFIG_SYS_IMAGE_EXTENDER_ADDR);
	if (img_size == 0) {
			run_command("setenv checkfail 1;fw_recovery_extender", 0);
		return -1;
	}
	printf("image size:0x%x",img_size);

	board_flash_read((char*)load_addr,
	                 CONFIG_SYS_IMAGE_EXTENDER_ADDR, img_size);

	CMDFMT_chk_kernel_rootfs(cmdbuf, load_addr, rootfs_addr(load_addr), "", "setenv checkfail 1;fw_recovery_extender");

	printf("cmdbuf : %s", cmdbuf);
	return (run_command(cmdbuf, 0) == CMD_RET_SUCCESS) ? 0 : -1;
}

int do_load_chk_dniimg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong load_addr = CONFIG_SYS_LOAD_ADDR;
	int bootpart;
	char buf[BOARD_BOOT_PARTITION_LENGTH + 1] = {0};
	int val;

	if(argc > 2)
		goto error;

	if (argv[1])
		load_addr = simple_strtoul(argv[1], NULL, 16);

	printf("Loading DNI firmware for checking...\n");
	get_board_data_2(BOARD_MODE_FLAG_OFFSET, BOARD_MODE_FLAG_LENGTH, buf);
	val = (int)simple_strtol(buf, NULL, 16);
	if (val == 2){
		load_chk_dniimg_extender(load_addr);
	} else{
		bootpart = boot_partition();
		if (bootpart == 1) {
			if (_load_chk_dniimg(load_addr, 1, 0) != 0)
				_load_chk_dniimg(load_addr, 2, 1);
		} else if (bootpart == 2) {
			if (_load_chk_dniimg(load_addr, 2, 0) != 0)
				_load_chk_dniimg(load_addr, 1, 1);
	}
	}
	return 0;

error:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	load_chk_dniimg, 2, 0, do_load_chk_dniimg,
	"load & check integrity of dni firmware image.",
	"<addr> - load (from flash to ram <addr>) & check integrity of dni firmware image.\n"
	"    <addr> : addr the image loaded to. '-' means CONFIG_SYS_LOAD_ADDR. \n"
);

int board_get_image_prdl (ulong fw_image_addr)
{
    char board_hw_id[10 + 1];
    char image_hw_id[10 + 1];

    /*get hardward id from board
    memset(board_hw_id, 0, sizeof(board_hw_id));
    get_board_data(BOARD_HW_ID_OFFSET, BOARD_HW_ID_LENGTH, (u8 *)board_hw_id);
    printf("HW ID on board: %s\n", board_hw_id);
    */

    /*get hardward id from image */
    memset(image_hw_id, 0, sizeof(image_hw_id));
    board_get_image_info(fw_image_addr, "PRDL", image_hw_id);
    printf("PRDL on image: %s\n", image_hw_id);

    return simple_strtol(image_hw_id, NULL, 16);
}

void do_board_firmware_corrupted_Ledloop(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 0);
        int i=0;
        for(i;;i++){
		if(tstc())
			break;
		if( i%2 == 0)
			printf("hit anykey to take control\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		else 
			printf("                          \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
                ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 1);
                mdelay(250);
                ipq40xx_handle_gpio(GPIO_SET, POWER_LED_RED, 0);
                mdelay(750);
        }
        ipq40xx_handle_gpio(GPIO_SET, POWER_LED_GREEN, 1);
}

U_BOOT_CMD(
	board_firmware_corrupted_Ledloop, 1, 0, do_board_firmware_corrupted_Ledloop,
	"power LED red blink loop",
	"power LED red blink loop \n"
);
