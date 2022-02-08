#
# Copyright (C) 2008-2020 Delta Networks Inc.
#
# this software are disgned by dni
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-chat
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

LOCAL_PKG_NAME_EXT:=dni-chat

include $(INCLUDE_DIR)/package.mk

define Package/dni-chat
  SECTION:=net
  CATEGORY:=DNI Package
  TITLE:=dni-chat
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src $(PKG_BUILD_DIR)/src
	$(CP) ./files $(PKG_BUILD_DIR)/files
endef

define Build/Compile
endef

define Package/dni-chat/install
	install -d -m0755 $(1)/usr/sbin
	install -d -m0755 $(1)/etc/init.d
	install -m0755 $(PKG_BUILD_DIR)/src/* $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/files/mbctrl.sh $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/files/dnimobile $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,dni-chat))
