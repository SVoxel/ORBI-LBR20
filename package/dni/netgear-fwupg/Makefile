#
## Copyright (C) 2019-2020 Delta Networks Inc.
#
## this software are disgned by dni by iceway.zhang
#
 
include $(TOPDIR)/rules.mk

PKG_NAME:=netgear-fwupg
PKG_RELEASE:=1

PKG_GIT_TREEISH=netgear-fwupg-v1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=dni
  CATEGORY:=DNI-Package
  TITLE:=$(PKG_NAME)
  DEPENDS:=+curl +tar +mtd-utils-nandwrite +mtd-utils-nanddump +artmtd +datalib +led-control \
	+@DNIBUSYBOX_CONFIG_PING +@DNIBUSYBOX_CONFIG_DD
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)/files/
	mkdir -p $(PKG_BUILD_DIR)/src/
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
	$(CP) ./files/* $(PKG_BUILD_DIR)/files/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/* $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/fwupgd.init $(1)/etc/init.d/fwupgd
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
