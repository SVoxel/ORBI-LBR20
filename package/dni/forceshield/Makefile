include $(TOPDIR)/rules.mk
PKG_NAME:=forceshield
PKG_VERSION:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

LOCAL_PKG_NAME_EXT:=forceshield

include $(INCLUDE_DIR)/package.mk

define Package/forceshield
	SECTION:=net
	CATEGORY:=Network
	TITLE:=forceshield
	DEPENDS:=+libpthread +libopenssl +libstdcpp +datalib
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)/files
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ./files/* $(PKG_BUILD_DIR)/files
	$(call Build/Patch/Default)
	$(if $(PLATFORM_PATCH_DIR), $(call PatchDir,$(PKG_BUILD_DIR),$(PLATFORM_PATCH_DIR),))
	$(call Dni_Prepare_Ext,$(LOCAL_PKG_NAME_EXT))
endef

define Build/Compile
endef

define Package/forceshield/install
	install -d -m0755 $(1)/etc/init.d
	$(LN) /tmp/media $(1)/media
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/forceshield_init $(1)/etc/init.d/
	install -d -m0755 $(1)/etc
	install -m0755 $(PKG_BUILD_DIR)/files/dap.zip $(1)/etc/
	install -m0755 $(PKG_BUILD_DIR)/files/verify_dap $(1)/etc/
	install -d -m0755 $(1)/etc/dap
	install -m0755 $(PKG_BUILD_DIR)/files/check_dap.sh $(1)/etc/dap/
	install -m0755 $(PKG_BUILD_DIR)/files/dap_port_init.sh $(1)/etc/dap/
	install -m0755 $(PKG_BUILD_DIR)/files/dap_port_reinit.sh $(1)/etc/dap/
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/files/forceshield_pre_update.sh $(1)/sbin/
	install -m0755 $(PKG_BUILD_DIR)/files/forceshield_update.sh $(1)/sbin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/forceshield_update $(1)/usr/sbin/
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,forceshield))
