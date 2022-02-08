#
# Copyright (C) 2019 Quectel.
#

include $(TOPDIR)/rules.mk
PKG_NAME:=quectel-CM
PKG_VERSION:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

CONFIG_QUECTELCM_GIT_TREEISH="qCM_v1.5.2-dni4"
CONFIG_QUECTELCM_GIT_REPOSITORY="quectel-CM.git"

PKG_GIT_TREEISH:=$(CONFIG_QUECTELCM_GIT_TREEISH)

LOCAL_PKG_NAME_EXT:=quectel-CM

include $(INCLUDE_DIR)/package.mk

define Package/quectel-CM
	SECTION:=net
	CATEGORY:=Network
	TITLE:=quectel-CM
	DEPENDS:=+libpthread +libdl +datalib
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/quectel-CM/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/quectel-CM $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/quectel-qmi-proxy $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,quectel-CM))
