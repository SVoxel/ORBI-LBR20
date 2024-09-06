#
# Copyright (C) 2022 Voxel
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sms-tool
PKG_VERSION:=1.03
PKG_RELEASE:=1

PKG_SOURCE_VERSION:=fce2b931c8d749c28b8281363950e963c98324eb
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
SMS_TOOL_GIT_REPOSITORY:="sms-tool.git"

PKG_LICENSE:=Apache-2.0

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os

define Package/sms-tool
	SECTION:=Utils
	CATEGORY:=Utilities
	TITLE:=sms-tool
	URL:=https://github.com/obsy/sms_tool
endef

define Package/sms-tool/description
	Small utility to manage SMS.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/$(SMS_TOOL_GIT_REPOSITORY)/* $(PKG_BUILD_DIR)/
	$(CP) ./patches $(PKG_BUILD_DIR)/
	$(PATCH) -i./patches/001-LBR20.patch -d$(PKG_BUILD_DIR)
endef

define Package/sms-tool/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sms_tool $(1)/usr/bin/sms-tool
endef

$(eval $(call BuildPackage,sms-tool))
