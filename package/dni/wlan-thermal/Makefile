# 
# Copyright (C) 2019-2020 Delta Networks Inc.
#
# this software are disgned by dni
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wlan-thermal
PKG_VERSION:=1.0.1
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=wlan thermal OTP & RA tools
  DEPENDS:=+libjson-c +libuci
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ./files/* $(PKG_BUILD_DIR)/
	$(call Build/Patch/Default)
	$(if $(PLATFORM_PATCH_DIR), $(call PatchDir,$(PKG_BUILD_DIR),$(PLATFORM_PATCH_DIR),))
	$(call Dni_Prepare_Ext,$(LOCAL_PKG_NAME_EXT))
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/etc/* $(1)/etc
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sbin/* $(1)/sbin
endef

$(eval $(call BuildPackage,wlan-thermal))
