/*
 * Copyright (c) 2013-2014 The Linux Foundation. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <asm/arch-ipq806x/smem.h>
#if defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4)
#include <asm/sizes.h>
#include <asm/arch-ipq806x/scm.h>
#include <asm/arch-ipq806x/iomap.h>
#endif

struct dumpinfo_t {
	char name[16]; /* use only file name in 8.3 format */
	uint32_t start;
	uint32_t size;
	int is_aligned_access; /* non zero represent 4 byte access */
};

struct dumpinfo_t dumpinfo[] = {
	/* Note1: when aligned access is set, the contents
	 * are copied to a temporary location and so
	 * the size of region should not exceed the size
	 * of region pointed by IPQ_TEMP_DUMP_ADDR
	 *
	 * Note2: IPQ_NSSTCM_DUMP_ADDR should be the
	 * first entry */
	{ "NSSTCM.BIN",   IPQ_NSSTCM_DUMP_ADDR, 0x20000, 0 },
	{ "IMEM_A.BIN",   0x2a000000, 0x0003f000, 0 },
	{ "IMEM_C.BIN",   0x2a03f000, 0x00001000, 0 },
	{ "IMEM_D.BIN",   0x2A040000, 0x00020000, 0 },
	{ "CODERAM.BIN",  0x00020000, 0x00028000, 0 },
	{ "SPS_RAM.BIN",  0x12000000, 0x0002C000, 0 },
	{ "RPM_MSG.BIN",  0x00108000, 0x00005fff, 1 },
	{ "SPS_BUFF.BIN", 0x12040000, 0x00004000, 0 },
	{ "SPS_PIPE.BIN", 0x12800000, 0x00008000, 0 },
	{ "LPASS.BIN",    0x28400000, 0x00020000, 0 },
	{ "RPM_WDT.BIN",  0x0006206C, 0x00000004, 0 },
	{ "CPU0_WDT.BIN", 0x0208A044, 0x00000004, 0 },
	{ "CPU1_WDT.BIN", 0x0209A044, 0x00000004, 0 },
#if defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4)
	{ "CPU0_REG.BIN", 0x39013ea8, 0x000000AC, 0 },
	{ "CPU1_REG.BIN", 0x39013f54, 0x000000AC, 0 },
#endif
	{ "WLAN_FW.BIN",  0x41400000, 0x000FFF80, 0 },
#if defined(CONFIG_HW29764958P0P128P512P3X3P4X4) || \
    defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4) || \
    defined(CONFIG_HW29764958P0P128P512P4X4P4X4PXDSL)
	{ "WLAN_FW_900B.BIN", 0x44000000, 0x00600000, 0 },
#endif
	{ "EBICS0.BIN",   0x40000000, 0x20000000, 0 },
	{ "EBI1CS1.BIN",  0x60000000, 0x20000000, 0 }
};

#if defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4)
void forever(void) { while (1); }
/*
 * Set the cold/warm boot address for one of the CPU cores.
 */
int scm_set_boot_addr(void)
{
	int ret;
	struct {
		unsigned int flags;
		unsigned long addr;
	} cmd;

	cmd.addr = (unsigned long)forever;
	cmd.flags = SCM_FLAG_COLDBOOT_CPU1;

	ret = scm_call(SCM_SVC_BOOT, SCM_BOOT_ADDR,
			&cmd, sizeof(cmd), NULL, 0);
	if (ret) {
		printf("--- %s: scm_call failed ret = %d\n", __func__, ret);
	}

	return ret;
}

static int krait_release_secondary(void)
{
	writel(0xa4, CPU1_APCS_SAW2_VCTL);
	barrier();
	udelay(512);

	writel(0x109, CPU1_APCS_CPU_PWR_CTL);
	writel(0x101, CPU1_APCS_CPU_PWR_CTL);
	barrier();
	udelay(1);

	writel(0x121, CPU1_APCS_CPU_PWR_CTL);
	barrier();
	udelay(2);

	writel(0x120, CPU1_APCS_CPU_PWR_CTL);
	barrier();
	udelay(2);

	writel(0x100, CPU1_APCS_CPU_PWR_CTL);
	barrier();
	udelay(100);

	writel(0x180, CPU1_APCS_CPU_PWR_CTL);
	barrier();

	return 0;
}
#endif

static int do_dumpipq_data(cmd_tbl_t *cmdtp, int flag, int argc,
				char *const argv[])
{
	char runcmd[128];
	char *serverip = NULL;
	char *dumpdir = ""; /* dump to root of TFTP server if none specified */
	int indx = 0;
	uint32_t memaddr = 0;

	if (argc == 2) {
		serverip = argv[1];
		printf("Using given serverip %s\n", serverip);
		setenv("serverip", serverip);
	} else {
		serverip = getenv("serverip");
		if (serverip != NULL) {
			printf("Using serverip from env %s\n", serverip);
		} else {
			printf("\nServer ip not found, run dhcp or configure\n");
			return CMD_RET_FAILURE;
		}
	}

	if (getenv("dumpdir") != NULL) {
		dumpdir = getenv("dumpdir");
		printf("Using directory %s in TFTP server\n", dumpdir);
	} else {
		printf("Env 'dumpdir' not set. Using / dir in TFTP server\n");
	}

#if defined(CONFIG_HW29764958P0P128P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P256P512P4X4P4X4PCASCADE) || \
    defined(CONFIG_HW29764958P0P128P1024P0P0) || \
    defined(CONFIG_HW29765257P0P128P256P3X3P4X4)
	if (scm_set_boot_addr() == 0) {
		/* Pull Core-1 out of reset, iff scm call succeeds */
		krait_release_secondary();
	}
#endif


	for (indx = 0; indx < ARRAY_SIZE(dumpinfo); indx++) {
		printf("\nProcessing %s:", dumpinfo[indx].name);
		memaddr = dumpinfo[indx].start;

		if (dumpinfo[indx].is_aligned_access) {
			snprintf(runcmd, sizeof(runcmd), "cp.l 0x%x 0x%x 0x%x",
				memaddr, IPQ_TEMP_DUMP_ADDR,
				dumpinfo[indx].size);

			if (run_command(runcmd, 0) != CMD_RET_SUCCESS)
				return CMD_RET_FAILURE;

			memaddr = IPQ_TEMP_DUMP_ADDR;
		}

		snprintf(runcmd, sizeof(runcmd), "tftpput 0x%x 0x%x %s/%s",
			memaddr, dumpinfo[indx].size,
			dumpdir, dumpinfo[indx].name);
		if (run_command(runcmd, 0) != CMD_RET_SUCCESS)
			return CMD_RET_FAILURE;
		udelay(10000); /* give some delay for server */
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(dumpipq_data, 2, 0, do_dumpipq_data,
	   "dumpipq_data crashdump collection from memory",
	   "dumpipq_data [serverip] - Crashdump collection from memory vi tftp\n");
