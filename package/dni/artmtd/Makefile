# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6501 2007-03-04 04:41:46Z pavlov $

include $(TOPDIR)/rules.mk

PKG_NAME:=artmtd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

LOCAL_PKG_NAME_EXT:=artmtd

include $(INCLUDE_DIR)/package.mk

define Package/artmtd
  SECTION:=utils
  CATEGORY:=Base system
  DEPENDS:= +dnitoolbox
  TITLE:=Read mac , wpspin, serialnumber and etc from art data
endef

define Package/artmtd/config
	menu "Configuration"
		depends on PACKAGE_artmtd
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	echo "/* It is auto generated and can be configured via \"make menuconfig\". */" > $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_SUPPORT_BOARDDATA2)x" == "x" || echo "#define BOARD_SUPPORT_BOARDDATA2 $(CONFIG_SUPPORT_BOARDDATA2)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_SERIAL_NUMBER_OFFSET)x" == "x" || echo "#define BOARD_SERIAL_NUMBER_OFFSET $(CONFIG_SERIAL_NUMBER_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_SERIAL_NUMBER_LENGTH)x" == "x" || echo "#define BOARD_SERIAL_NUMBER_LENGTH $(CONFIG_SERIAL_NUMBER_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_REGION_NUMBER_OFFSET)x" == "x" || echo "#define BOARD_REGION_NUMBER_OFFSET $(CONFIG_REGION_NUMBER_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_REGION_NUMBER_LENGTH)x" == "x" || echo "#define BOARD_REGION_NUMBER_LENGTH $(CONFIG_REGION_NUMBER_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_HW_ID_OFFSET)x" == "x" || echo "#define BOARD_BOARD_HW_ID_OFFSET $(CONFIG_BOARD_HW_ID_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_HW_ID_LENGTH)x" == "x" || echo "#define BOARD_BOARD_HW_ID_LENGTH $(CONFIG_BOARD_HW_ID_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_MODEL_ID_OFFSET)x" == "x" || echo "#define BOARD_BOARD_MODEL_ID_OFFSET $(CONFIG_BOARD_MODEL_ID_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_MODEL_ID_LENGTH)x" == "x" || echo "#define BOARD_BOARD_MODEL_ID_LENGTH $(CONFIG_BOARD_MODEL_ID_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_FRONTHAUL_SSID_OFFSET)x" == "x" || echo "#define BOARD_BOARD_FRONTHAUL_SSID_OFFSET $(CONFIG_BOARD_FRONTHAUL_SSID_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_FRONTHAUL_SSID_LENGTH)x" == "x" || echo "#define BOARD_BOARD_FRONTHAUL_SSID_LENGTH $(CONFIG_BOARD_FRONTHAUL_SSID_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BACKHAUL_SSID_OFFSET)x" == "x" || echo "#define BOARD_BOARD_BACKHAUL_SSID_OFFSET $(CONFIG_BOARD_BACKHAUL_SSID_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BACKHAUL_SSID_LENGTH)x" == "x" || echo "#define BOARD_BOARD_BACKHAUL_SSID_LENGTH $(CONFIG_BOARD_BACKHAUL_SSID_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_FRONTHAUL_PASSPHRASE_OFFSET)x" == "x" || echo "#define BOARD_BOARD_FRONTHAUL_PASSPHRASE_OFFSET $(CONFIG_BOARD_FRONTHAUL_PASSPHRASE_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_FRONTHAUL_PASSPHRASE_LENGTH)x" == "x" || echo "#define BOARD_BOARD_FRONTHAUL_PASSPHRASE_LENGTH $(CONFIG_BOARD_FRONTHAUL_PASSPHRASE_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BACKHAUL_PASSPHRASE_OFFSET)x" == "x" || echo "#define BOARD_BOARD_BACKHAUL_PASSPHRASE_OFFSET $(CONFIG_BOARD_BACKHAUL_PASSPHRASE_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BACKHAUL_PASSPHRASE_LENGTH)x" == "x" || echo "#define BOARD_BOARD_BACKHAUL_PASSPHRASE_LENGTH $(CONFIG_BOARD_BACKHAUL_PASSPHRASE_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_DATA_OFFSET)x" == "x" || echo "#define BOARD_BOARD_DATA_OFFSET $(CONFIG_BOARD_DATA_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_DGC_SUPPORT_NAND_FLASH)x" == "x" || echo "#define BOARD_SUPPORT_NAND_TYPE $(CONFIG_DGC_SUPPORT_NAND_FLASH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_DGC_SUPPORT_NOR_FLASH)x" == "x" || echo "#define BOARD_SUPPORT_NOR_TYPE $(CONFIG_DGC_SUPPORT_NOR_FLASH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_DGC_SUPPORT_EMMC)x" == "x" || echo "#define BOARD_SUPPORT_EMMC_TYPE $(CONFIG_DGC_SUPPORT_EMMC)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_DATA_LENGTH)x" == "x" || echo "#define BOARD_BOARD_DATA_LENGTH $(CONFIG_BOARD_DATA_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOOT_PART_OFFSET)x" == "x" || echo "#define BOARD_BOOT_PART_OFFSET $(CONFIG_BOOT_PART_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOOT_PART_LENGTH)x" == "x" || echo "#define BOARD_BOOT_PART_LENGTH $(CONFIG_BOOT_PART_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_ART_SIZE)x" == "x" || echo "#define BOARD_ART_SIZE $(CONFIG_ART_SIZE)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_REGION_FILE)x" == "x" || echo "#define BOARD_REGION_FILE \"$(CONFIG_REGION_FILE)\"" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_CAL_BLOCK_NUM)x" == "x" || echo "#define BOARD_CAL_BLOCK_NUM $(CONFIG_CAL_BLOCK_NUM)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_MTD_BLOCK_SIZE)x" == "x" || echo "#define BOARD_MTD_BLOCK_SIZE $(CONFIG_MTD_BLOCK_SIZE)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BOOT_PARTITION_OFFSET)x" == "x" || echo "#define BOARD_BOARD_BOOT_PARTITION_OFFSET $(CONFIG_BOARD_BOOT_PARTITION_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_BOARD_BOOT_PARTITION_LENGTH)x" == "x" || echo "#define BOARD_BOARD_BOOT_PARTITION_LENGTH $(CONFIG_BOARD_BOOT_PARTITION_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_HW_REVISION_OFFSET)x" == "x" || echo "#define BOARD_HW_REVISION_OFFSET $(CONFIG_HW_REVISION_OFFSET)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_HW_REVISION_LENGTH)x" == "x" || echo "#define BOARD_HW_REVISION_LENGTH $(CONFIG_HW_REVISION_LENGTH)" >> $(PKG_BUILD_DIR)/config.h
	test "$(CONFIG_DGC_HW_ID)x" == "x" || echo "#define BOARD_BOARD_HW_ID_DEFAULT \"$(CONFIG_DGC_HW_ID)\"" >> $(PKG_BUILD_DIR)/config.h
endef

define Build/Compile
endef

define Package/artmtd/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/artmtd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/artmtd-2 $(1)/sbin/
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,artmtd))
