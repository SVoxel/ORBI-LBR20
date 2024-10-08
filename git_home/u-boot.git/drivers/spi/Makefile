#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libspi.o

COBJS-$(CONFIG_ALTERA_SPI) += altera_spi.o
COBJS-$(CONFIG_ANDES_SPI) += andes_spi.o
COBJS-$(CONFIG_ARMADA100_SPI) += armada100_spi.o
COBJS-$(CONFIG_ATMEL_DATAFLASH_SPI) += atmel_dataflash_spi.o
COBJS-$(CONFIG_ATMEL_SPI) += atmel_spi.o
COBJS-$(CONFIG_BFIN_SPI) += bfin_spi.o
COBJS-$(CONFIG_CF_SPI) += cf_spi.o
COBJS-$(CONFIG_DAVINCI_SPI) += davinci_spi.o
COBJS-$(CONFIG_KIRKWOOD_SPI) += kirkwood_spi.o
COBJS-$(CONFIG_MPC52XX_SPI) += mpc52xx_spi.o
COBJS-$(CONFIG_MPC8XXX_SPI) += mpc8xxx_spi.o
COBJS-$(CONFIG_MXC_SPI) += mxc_spi.o
COBJS-$(CONFIG_MXS_SPI) += mxs_spi.o
COBJS-$(CONFIG_OC_TINY_SPI) += oc_tiny_spi.o
COBJS-$(CONFIG_OMAP3_SPI) += omap3_spi.o
COBJS-$(CONFIG_SOFT_SPI) += soft_spi.o
COBJS-$(CONFIG_SH_SPI) += sh_spi.o
COBJS-$(CONFIG_FSL_ESPI) += fsl_espi.o
COBJS-$(CONFIG_TEGRA_SPI) += tegra_spi.o
COBJS-$(CONFIG_IPQ_SPI) += ipq_spi.o
COBJS-$(CONFIG_HW29765265P16P0P256P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765285P16P0P128) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4PVERIZON) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765641P0P256P512P2X2P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765641P0P128P512P2X2P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765641P0P128P256P2X2P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29766182P0P256P512P2X2P2X2P2X2P1) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29766105P0P128P512P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765906P0P256P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4PVERIZON) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4PVERIZON) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2) += qca_qup_spi_bam.o
COBJS-$(CONFIG_HW29766106P32P0P256P2X2P2X2) += qca_qup_spi_bam.o

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
