# 
#  Copyright (C) 2011 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=phddns
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
#PKG_SOURCE_URL:=http://open.oray.com/doc/phddns-sdk-1.0.4.17314.tar.gz

LOCAL_PKG_NAME_EXT:=phddns

include $(INCLUDE_DIR)/package.mk

define Package/phddns
	SECTION:=utils
	CATEGORY:=Network
	DEPENDS:=+datalib
	TITLE:=Oray DDNS utility.
endef

define Build/Prepare
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
	$(call Build/Patch/Default)
	$(if $(PLATFORM_PATCH_DIR), $(call PatchDir,$(PKG_BUILD_DIR),$(PLATFORM_PATCH_DIR),))
	$(call Dni_Prepare_Ext,$(LOCAL_PKG_NAME_EXT))
endef

define Package/phddns/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/phddns $(1)/usr/sbin/
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,phddns))
