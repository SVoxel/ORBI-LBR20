#
# (C) Copyright 2006
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

LIB	:= $(obj)libdma.o

COBJS-$(CONFIG_FSLDMAFEC) += MCD_tasksInit.o MCD_dmaApi.o MCD_tasks.o
COBJS-$(CONFIG_APBH_DMA) += apbh_dma.o
COBJS-$(CONFIG_FSL_DMA) += fsl_dma.o
COBJS-$(CONFIG_OMAP3_DMA) += omap3_dma.o
COBJS-$(CONFIG_HW29765265P16P0P256P2X2P2X2)  += bam.o
COBJS-$(CONFIG_HW29765285P16P0P128) += bam.o
COBJS-$(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765352P32P4000P512P2X2P2X2P4X4PVERIZON) += bam.o
COBJS-$(CONFIG_HW29765352P0P4096P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765352P32P0P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765619P0P256P512P2X2P2X2P4X4PVERIZON) += bam.o
COBJS-$(CONFIG_HW29765641P0P256P512P2X2P2X2P2X2) += bam.o
COBJS-$(CONFIG_HW29765641P0P128P512P2X2P2X2P2X2) += bam.o
COBJS-$(CONFIG_HW29765641P0P128P256P2X2P2X2P2X2) += bam.o
COBJS-$(CONFIG_HW29766182P0P256P512P2X2P2X2P2X2P1) += bam.o
COBJS-$(CONFIG_HW29766105P0P128P512P2X2P2X2) += bam.o
COBJS-$(CONFIG_HW29765906P0P256P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765913P0P512P512P2X2P2X2P4X4PVERIZON) += bam.o
COBJS-$(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4) += bam.o
COBJS-$(CONFIG_HW29765913P0P128P256P2X2P2X2P4X4PVERIZON) += bam.o
COBJS-$(CONFIG_HW29765515P0P4096P512P2X2P2X2P2X2) += bam.o
COBJS-$(CONFIG_HW29766106P32P0P256P2X2P2X2) += bam.o

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
