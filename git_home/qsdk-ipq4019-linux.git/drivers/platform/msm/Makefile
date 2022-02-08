#
# Makefile for the MSM specific device drivers.
#

KBUILD_CFLAGS := $(filter-out -O%,$(KBUILD_CFLAGS)) -O3

obj-$(CONFIG_SPS) += sps/
obj-$(CONFIG_MSM_BUS_SCALING) += msm_bus/
obj-$(CONFIG_BUS_TOPOLOGY_ADHOC) += msm_bus/
obj-$(CONFIG_BOOTCONFIG_PARTITION) += bootconfig.o
obj-$(CONFIG_I2C_MSM_PROF_DBG) += i2c-msm-prof-dbg.o
