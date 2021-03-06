/*
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *  Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *  Copyright (c) 2014 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/i2c.h>

#include <asm/smp_plat.h>

#include <asm/cacheflush.h>
#include "scm-boot.h"

#define VDD_SC1_ARRAY_CLAMP_GFS_CTL	0x35a0
#define SCSS_CPU1CORE_RESET		0x2d80
#define SCSS_DBG_STATUS_CORE_PWRDUP	0x2e64

#define APCS_CPU_PWR_CTL	0x04
#define PLL_CLAMP		BIT(8)
#define CORE_PWRD_UP		BIT(7)
#define COREPOR_RST		BIT(5)
#define CORE_RST		BIT(4)
#define L2DT_SLP		BIT(3)
#define CLAMP			BIT(0)

#define APC_PWR_GATE_CTL	0x14
#define BHS_CNT_SHIFT		24
#define LDO_PWR_DWN_SHIFT	16
#define LDO_BYP_SHIFT		8
#define BHS_SEG_SHIFT		1
#define BHS_EN			BIT(0)

#define APCS_SAW2_VCTL		0x14
#define APCS_SAW2_2_VCTL	0x1c
#define of_board_is_sim()	(of_machine_is_compatible("qcom,qca961x-r3pc") \
				 || of_machine_is_compatible("qcom,qca961x-virtio"))

extern void secondary_startup(void);
extern void msm_secondary_startup(void);
extern char *flash_type_name;

static DEFINE_SPINLOCK(boot_lock);

static struct i2c_board_info __initdata i2c_devices[] ={
	{
		I2C_BOARD_INFO("tlc59208f",0x27),
	},
};

#if CONFIG_SENSOR_FUNCTION_SUPPORT_DNI
static struct i2c_board_info __initdata i2c_2_devices[] ={
	{
		I2C_BOARD_INFO("sensor",0x48),
	},
};

static struct i2c_board_info __initdata i2c_3_devices[] ={
	{
		I2C_BOARD_INFO("sensor2",0x4e),
	},
};
#endif

#ifdef CONFIG_HOTPLUG_CPU
static void __ref qcom_cpu_die(unsigned int cpu)
{
	wfi();
}
#endif

/*
 * Write pen_release in a way that is guaranteed to be visible to all
 * observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
void __cpuinit write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));
}

static int __cpuinit release_from_pen(unsigned int cpu)
{
	unsigned long timeout;

	/* Set preset_lpj to avoid subsequent lpj recalculations */
	preset_lpj = loops_per_jiffy;
	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	write_pen_release(cpu_logical_map(cpu));

	if (of_board_is_sim())
		/* simulation platforms are very slow */
		timeout = jiffies + 1;
	else
		timeout = jiffies + (1 * HZ);

	while (time_before(jiffies, timeout)) {
		smp_rmb(); /* barrier */
		if (pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

static void qcom_secondary_init(unsigned int cpu)
{
	i2c_register_board_info(0,i2c_devices,ARRAY_SIZE(i2c_devices));
#if CONFIG_SENSOR_FUNCTION_SUPPORT_DNI
	if (strcmp(flash_type_name,"EMMC") != 0){
		i2c_register_board_info(0,i2c_2_devices,ARRAY_SIZE(i2c_2_devices));
		i2c_register_board_info(0,i2c_3_devices,ARRAY_SIZE(i2c_3_devices));
	}
#endif
	printk(" \n i2c devices info\n");
	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	if (of_board_is_sim())
		write_pen_release(-1);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

static int scss_release_secondary(unsigned int cpu)
{
	struct device_node *node;
	void __iomem *base;

	node = of_find_compatible_node(NULL, NULL, "qcom,gcc-msm8660");
	if (!node) {
		pr_err("%s: can't find node\n", __func__);
		return -ENXIO;
	}

	base = of_iomap(node, 0);
	of_node_put(node);
	if (!base)
		return -ENOMEM;

	writel_relaxed(0, base + VDD_SC1_ARRAY_CLAMP_GFS_CTL);
	writel_relaxed(0, base + SCSS_CPU1CORE_RESET);
	writel_relaxed(3, base + SCSS_DBG_STATUS_CORE_PWRDUP);
	mb();
	iounmap(base);

	return 0;
}

static int __cpuinit a7ss_release_secondary(unsigned int cpu)
{
	struct device_node *node;
	void __iomem *base;
	struct resource res;

	node = of_find_compatible_node(NULL, NULL, "qcom,arm-cortex-acc");
	if (!node) {
		pr_err("%s: can't find node\n", __func__);
		return -ENXIO;
	}

	if (of_address_to_resource(node, 0, &res)) {
		of_node_put(node);
		return -ENXIO;
	}

	res.start += cpu * 0x10000;

	base = ioremap(res.start, 0x1000);
	of_node_put(node);

	if (!base)
		return -ENOMEM;


	if (!of_board_is_sim()) {
		/* Enable Clamp signal and assert core reset */
		writel_relaxed(0x00000033, base + 0x04);
		mb(); /* barrier */

		/* Set GDHS and delay counter */
		writel_relaxed(0x20000001, base + 0x14);
		mb(); /* barrier */

		udelay(2);

		/* Enable Core memory HS */
		writel_relaxed(0x00020008, base + 0x04);
		mb(); /* barrier */

		/* Report that the CPU is powered up */
		writel_relaxed(0x00020088, base + 0x04);
		mb(); /* barrier */
	}

	iounmap(base);
	return 0;
}

static int kpssv1_release_secondary(unsigned int cpu)
{
	int ret = 0;
	void __iomem *reg, *saw_reg;
	struct device_node *cpu_node, *acc_node, *saw_node;
	u32 val;

	cpu_node = of_get_cpu_node(cpu, NULL);
	if (!cpu_node)
		return -ENODEV;

	acc_node = of_parse_phandle(cpu_node, "qcom,acc", 0);
	if (!acc_node) {
		ret = -ENODEV;
		goto out_acc;
	}

	saw_node = of_parse_phandle(cpu_node, "qcom,saw", 0);
	if (!saw_node) {
		ret = -ENODEV;
		goto out_saw;
	}

	reg = of_iomap(acc_node, 0);
	if (!reg) {
		ret = -ENOMEM;
		goto out_acc_map;
	}

	saw_reg = of_iomap(saw_node, 0);
	if (!saw_reg) {
		ret = -ENOMEM;
		goto out_saw_map;
	}

	/* Turn on CPU rail */
	writel_relaxed(0xA4, saw_reg + APCS_SAW2_VCTL);
	mb();
	udelay(512);

	/* Krait bring-up sequence */
	val = PLL_CLAMP | L2DT_SLP | CLAMP;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	val &= ~L2DT_SLP;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	mb();
	ndelay(300);

	val |= COREPOR_RST;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	mb();
	udelay(2);

	val &= ~CLAMP;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	mb();
	udelay(2);

	val &= ~COREPOR_RST;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	mb();
	udelay(100);

	val |= CORE_PWRD_UP;
	writel_relaxed(val, reg + APCS_CPU_PWR_CTL);
	mb();

	iounmap(saw_reg);
out_saw_map:
	iounmap(reg);
out_acc_map:
	of_node_put(saw_node);
out_saw:
	of_node_put(acc_node);
out_acc:
	of_node_put(cpu_node);
	return ret;
}

static int kpssv2_release_secondary(unsigned int cpu)
{
	void __iomem *reg;
	struct device_node *cpu_node, *l2_node, *acc_node, *saw_node;
	void __iomem *l2_saw_base;
	unsigned reg_val;
	int ret;

	cpu_node = of_get_cpu_node(cpu, NULL);
	if (!cpu_node)
		return -ENODEV;

	acc_node = of_parse_phandle(cpu_node, "qcom,acc", 0);
	if (!acc_node) {
		ret = -ENODEV;
		goto out_acc;
	}

	l2_node = of_parse_phandle(cpu_node, "next-level-cache", 0);
	if (!l2_node) {
		ret = -ENODEV;
		goto out_l2;
	}

	saw_node = of_parse_phandle(l2_node, "qcom,saw", 0);
	if (!saw_node) {
		ret = -ENODEV;
		goto out_saw;
	}

	reg = of_iomap(acc_node, 0);
	if (!reg) {
		ret = -ENOMEM;
		goto out_map;
	}

	l2_saw_base = of_iomap(saw_node, 0);
	if (!l2_saw_base) {
		ret = -ENOMEM;
		goto out_saw_map;
	}

	/* Turn on the BHS, turn off LDO Bypass and power down LDO */
	reg_val = (64 << BHS_CNT_SHIFT) | (0x3f << LDO_PWR_DWN_SHIFT) | BHS_EN;
	writel_relaxed(reg_val, reg + APC_PWR_GATE_CTL);
	mb();
	/* wait for the BHS to settle */
	udelay(1);

	/* Turn on BHS segments */
	reg_val |= 0x3f << BHS_SEG_SHIFT;
	writel_relaxed(reg_val, reg + APC_PWR_GATE_CTL);
	mb();
	 /* wait for the BHS to settle */
	udelay(1);

	/* Finally turn on the bypass so that BHS supplies power */
	reg_val |= 0x3f << LDO_BYP_SHIFT;
	writel_relaxed(reg_val, reg + APC_PWR_GATE_CTL);

	/* enable max phases */
	writel_relaxed(0x10003, l2_saw_base + APCS_SAW2_2_VCTL);
	mb();
	udelay(50);

	reg_val = COREPOR_RST | CLAMP;
	writel_relaxed(reg_val, reg + APCS_CPU_PWR_CTL);
	mb();
	udelay(2);

	reg_val &= ~CLAMP;
	writel_relaxed(reg_val, reg + APCS_CPU_PWR_CTL);
	mb();
	udelay(2);

	reg_val &= ~COREPOR_RST;
	writel_relaxed(reg_val, reg + APCS_CPU_PWR_CTL);
	mb();

	reg_val |= CORE_PWRD_UP;
	writel_relaxed(reg_val, reg + APCS_CPU_PWR_CTL);
	mb();

	ret = 0;

	iounmap(l2_saw_base);
out_saw_map:
	iounmap(reg);
out_map:
	of_node_put(saw_node);
out_saw:
	of_node_put(l2_node);
out_l2:
	of_node_put(acc_node);
out_acc:
	of_node_put(cpu_node);

	return ret;
}

static DEFINE_PER_CPU(int, cold_boot_done);

static int qcom_boot_secondary(unsigned int cpu, int (*func)(unsigned int))
{
	int ret = 0;

	if (!per_cpu(cold_boot_done, cpu)) {
		ret = func(cpu);
		if (!ret)
			per_cpu(cold_boot_done, cpu) = true;
	}

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return ret;
}

static int msm8660_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	return qcom_boot_secondary(cpu, scss_release_secondary);
}

static int a7ss_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int ret = 0;

	ret = qcom_boot_secondary(cpu, a7ss_release_secondary);

	if (of_board_is_sim())
		release_from_pen(cpu);

	return ret;
}

static int kpssv1_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	return qcom_boot_secondary(cpu, kpssv1_release_secondary);
}

static int kpssv2_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	return qcom_boot_secondary(cpu, kpssv2_release_secondary);
}

static void __init qcom_smp_prepare_cpus(unsigned int max_cpus)
{
	int cpu, map;
	unsigned int flags = 0;
	phys_addr_t phys;

	static const int cold_boot_flags[] = {
		0,
		SCM_FLAG_COLDBOOT_CPU1,
		SCM_FLAG_COLDBOOT_CPU2,
		SCM_FLAG_COLDBOOT_CPU3,
	};

	for_each_present_cpu(cpu) {
		map = cpu_logical_map(cpu);
		if (WARN_ON(map >= ARRAY_SIZE(cold_boot_flags))) {
			set_cpu_present(cpu, false);
			continue;
		}
		flags |= cold_boot_flags[map];
	}

	if (of_board_is_sim())
		phys = virt_to_phys(msm_secondary_startup);
	else
		phys = virt_to_phys(secondary_startup);

	if (scm_set_boot_addr(phys, flags)) {
		for_each_present_cpu(cpu) {
			if (cpu == smp_processor_id())
				continue;
			set_cpu_present(cpu, false);
		}
		pr_warn("Failed to set CPU boot address, disabling SMP\n");
	}
}

static struct smp_operations smp_msm8660_ops __initdata = {
	.smp_prepare_cpus	= qcom_smp_prepare_cpus,
	.smp_secondary_init	= qcom_secondary_init,
	.smp_boot_secondary	= msm8660_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= qcom_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(qcom_smp, "qcom,gcc-msm8660", &smp_msm8660_ops);

static struct smp_operations qcom_smp_kpssv1_ops __initdata = {
	.smp_prepare_cpus	= qcom_smp_prepare_cpus,
	.smp_secondary_init	= qcom_secondary_init,
	.smp_boot_secondary	= kpssv1_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= qcom_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(qcom_smp_kpssv1, "qcom,kpss-acc-v1", &qcom_smp_kpssv1_ops);

static struct smp_operations qcom_smp_kpssv2_ops __initdata = {
	.smp_prepare_cpus	= qcom_smp_prepare_cpus,
	.smp_secondary_init	= qcom_secondary_init,
	.smp_boot_secondary	= kpssv2_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= qcom_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(qcom_smp_kpssv2, "qcom,kpss-acc-v2", &qcom_smp_kpssv2_ops);

static struct smp_operations qcom_smp_a7ss_ops __initdata = {
	.smp_prepare_cpus       = qcom_smp_prepare_cpus,
	.smp_secondary_init     = qcom_secondary_init,
	.smp_boot_secondary     = a7ss_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die                = qcom_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(qcom_smp_a7ss, "qcom,arm-cortex-acc", &qcom_smp_a7ss_ops);
