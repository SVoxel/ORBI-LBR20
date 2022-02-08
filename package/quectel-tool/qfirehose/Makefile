#
# Copyright (C) 2019 Quectel.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qfirehose
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/qfirehose
  SECTION:=utils
  CATEGORY:=Quectel Package
  DEPENDS:=+libpthread
  TITLE:=qfirehose Quectel for upgrade the FW
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	
endef

define Build/Compile
endef

define Package/qfirehose/install
	$(INSTALL_DIR) $(1)/usr/share/qfirehose
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/qfirehose $(1)/usr/share/qfirehose/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb2tcp $(1)/usr/share/qfirehose/

endef

$(eval $(call BuildPackage,qfirehose))
