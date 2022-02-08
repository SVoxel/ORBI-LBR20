#
# 	Copyright (C) 2019 Quectel.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qlogtool
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/qlogtool
  SECTION:=utils
  CATEGORY:=Quectel Package
  DEPENDS:=+libpthread
  TITLE:=QLog ql_tty2tcp Quectel log tool for LTE 
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/qlogtool/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ql_tty2tcp $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/QLog $(1)/usr/sbin/

endef

$(eval $(call BuildPackage,qlogtool))
