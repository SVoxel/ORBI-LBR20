#
# Copyright (C) 2010-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=openvpn

PKG_VERSION:=2.5.6
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://swupdate.openvpn.net/community/releases
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_MD5SUM:=434f02d3b371bf1dcd1e618e56969a4c

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CPPFLAGS:=-I$(STAGING_DIR)/usr/include/openssl-1.1.x $(TARGET_CPPFLAGS)
TARGET_LDFLAGS:=-L$(STAGING_DIR)/usr/lib/openssl-1.1.x $(TARGET_LDFLAGS)
TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -O3
TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

define Package/openvpn/Default
  TITLE:=Open source VPN solution using $(2)
  SECTION:=net
  CATEGORY:=Network
  URL:=http://openvpn.net
  SUBMENU:=VPN
  MENU:=1
  DEPENDS:=+kmod-tun +OPENVPN_$(1)_ENABLE_LZO:liblzo +OPENVPN_$(1)_ENABLE_LZ4:liblz4 +OPENVPN_$(1)_ENABLE_IPROUTE2:ip $(3)
  VARIANT:=$(1)
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
endef

Package/openvpn-openssl=$(call Package/openvpn/Default,openssl,OpenSSL,+libopenssl-1.1.x)
Package/openvpn-polarssl=$(call Package/openvpn/Default,polarssl,PolarSSL,+libpolarssl)
Package/openvpn-nossl=$(call Package/openvpn/Default,nossl,plaintext (no SSL))

define Package/openvpn/config/Default
	source "$(SOURCE)/Config-$(1).in"
endef

Package/openvpn-openssl/config=$(call Package/openvpn/config/Default,openssl)
Package/openvpn-polarssl/config=$(call Package/openvpn/config/Default,polarssl)
Package/openvpn-nossl/config=$(call Package/openvpn/config/Default,nossl)

ifeq ($(BUILD_VARIANT),polarssl)
CONFIG_OPENVPN_POLARSSL:=y
endif
ifeq ($(BUILD_VARIANT),openssl)
CONFIG_OPENVPN_OPENSSL:=y
endif
ifeq ($(BUILD_VARIANT),nossl)
CONFIG_OPENVPN_NOSSL:=y
endif

CONFIGURE_VARS += \
	IFCONFIG=/sbin/ifconfig \
	ROUTE=/sbin/route \
	IPROUTE=/usr/sbin/ip \
	NETSTAT=/sbin/netstat

define Build/Configure
	$(call Build/Configure/Default, \
		--enable-small \
		--disable-selinux \
		--disable-systemd \
		--disable-plugins \
		--disable-debug \
		--disable-eurephia \
		--disable-pkcs11 \
		--enable-password-save \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_LZO),--enable,--disable)-lzo \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_LZ4),--enable,--disable)-lz4 \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_X509_ALT_USERNAME),enable,disable-x509-alt-username)-ssl \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_SERVER),--enable,--disable)-server \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_MANAGEMENT),--enable,--disable)-management \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_SOCKS),--enable,--disable)-socks \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_HTTP),--enable,--disable)-http \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_FRAGMENT),--enable,--disable)-fragment \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_MULTIHOME),--enable,--disable)-multihome \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_IPROUTE2),--enable,--disable)-iproute2 \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_DEF_AUTH),--enable,--disable)-def-auth \
		$(if $(CONFIG_OPENVPN_$(BUILD_VARIANT)_ENABLE_PF),--enable,--disable)-pf \
		$(if $(CONFIG_OPENVPN_NOSSL),--disable-ssl --disable-crypto,--enable-ssl --enable-crypto) \
		$(if $(CONFIG_OPENVPN_OPENSSL),--with-crypto-library=openssl) \
		$(if $(CONFIG_OPENVPN_POLARSSL),--with-crypto-library=polarssl) \
	)
endef

define Package/openvpn-$(BUILD_VARIANT)/conffiles
/etc/config/openvpn
endef

define Package/openvpn-$(BUILD_VARIANT)/install
	$(INSTALL_DIR) \
		$(1)/usr/sbin \
		$(1)/etc/rc.d \
		$(1)/etc/init.d \
		$(1)/etc/config \
		$(1)/etc/openvpn \
		$(1)/lib/upgrade/keep.d

	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/sbin/openvpn \
		$(1)/usr/sbin/

	$(INSTALL_BIN) \
		files/openvpn.init \
		$(1)/etc/init.d/openvpn

	ln -sv ../init.d/openvpn $(1)/etc/rc.d/S60openvpn

	$(INSTALL_BIN) \
		files/openvpn-client.init \
		$(1)/etc/init.d/openvpn-client

	ln -sv ../init.d/openvpn $(1)/etc/rc.d/S99openvpn-client

	$(INSTALL_DATA) \
		files/openvpn.upgrade \
		$(1)/lib/upgrade/keep.d/openvpn
	$(INSTALL_BIN) \
		files/openvpn.push_routing_rule \
		$(1)/etc/openvpn/push_routing_rule
	$(INSTALL_BIN) \
		files/openvpn.download \
		$(1)/etc/openvpn/download
	$(INSTALL_BIN) \
		files/vpn-firewall.sh \
		$(1)/etc/openvpn/vpn-firewall.sh
	$(INSTALL_BIN) \
		files/ovpnclient-up.sh \
		$(1)/etc/openvpn/ovpnclient-up.sh
	$(INSTALL_BIN) \
		files/ovpnclient-down.sh \
		$(1)/etc/openvpn/ovpnclient-down.sh
endef

$(eval $(call BuildPackage,openvpn-openssl))
$(eval $(call BuildPackage,openvpn-polarssl))
$(eval $(call BuildPackage,openvpn-nossl))