# 
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-libmtd
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

LOCAL_PKG_NAME_EXT:=nmrp

include $(INCLUDE_DIR)/package.mk

define Package/dni-libmtd
  SECTION:=utils
  CATEGORY:=Utilities
  CATEGORY:=DNI Package
  TITLE:=Tiny utils for configuring the Router.
endef

define Package/dni-libmtd/description
	This package contains an utility for configuring Home Router.
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	#(call Dni_Prepare_Ext,$(LOCAL_PKG_NAME_EXT))
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmtd.so $(STAGING_DIR)/usr/lib/
endef

define Package/dni-libmtd/install
	install -d m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmtd.so $(1)/usr/lib
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,dni-libmtd))
