include $(TOPDIR)/rules.mk

PKG_NAME:=libubox
PKG_VERSION:=2015-11-08
PKG_RELEASE=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=http://git.openwrt.org/project/libubox.git
PKG_SOURCE_BRANCH:=openwrt/master
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=10429bccd0dc5d204635e110a7a8fae7b80d16cb
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_MIRROR_MD5SUM:=
CMAKE_INSTALL:=1

PKG_LICENSE:=ISC
PKG_LICENSE_FILES:=

PKG_MAINTAINER:=Felix Fietkau <nbd@openwrt.org>

PKG_BUILD_DEPENDS:=lua

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libubox
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Basic utility library
  ABI_VERSION:=$(PKG_VERSION)
  DEPENDS:=
endef

define Package/libblobmsg-json
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=blobmsg <-> json conversion library
  DEPENDS:=+libjson-c +libubox
endef

define Package/jshn
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libjson-c +libubox +libblobmsg-json
  TITLE:=JSON SHell Notation
endef

define Package/jshn/description
  Library for parsing and generating JSON from shell scripts
endef

define Package/libjson-script
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libubox
  TITLE:=Minimalistic JSON based scripting engine
endef

define Package/libubox-lua
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libubox +liblua
  TITLE:=Lua binding for the OpenWrt Basic utility library
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include -Wno-error=array-bounds
CMAKE_OPTIONS = \
	-DLUAPATH=/usr/lib/lua

define Package/libubox/install
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libubox.so $(1)/lib/
endef

define Package/libblobmsg-json/install
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libblobmsg_json.so $(1)/lib/
endef

define Package/jshn/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/usr/share/libubox
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/jshn $(1)/usr/bin
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/libubox/jshn.sh $(1)/usr/share/libubox
endef

define Package/libjson-script/install
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libjson_script.so $(1)/lib/
endef

define Package/libubox-lua/install
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(CP) $(PKG_BUILD_DIR)/lua/uloop.so $(1)/usr/lib/lua/
endef

$(eval $(call BuildPackage,libubox))
$(eval $(call BuildPackage,libblobmsg-json))
$(eval $(call BuildPackage,jshn))
$(eval $(call BuildPackage,libjson-script))
$(eval $(call BuildPackage,libubox-lua))
