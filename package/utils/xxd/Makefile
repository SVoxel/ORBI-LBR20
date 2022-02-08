#
# Copyright (C) 2022 Voxel
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xxd
PKG_VERSION:=1.10
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_MD5SUM:=604cf64093fddc30adf41f1f371c9032

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os
TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

define Package/xxd
	SECTION:=Utils
	CATEGORY:=Utilities
	TITLE:=xxd
endef

define Package/xxd/description
	Make a hexdump or do the reverse.
endef

define Package/xxd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xxd $(1)/usr/bin
endef

$(eval $(call BuildPackage,xxd))
