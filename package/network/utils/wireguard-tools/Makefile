#
# Copyright (C) 2016-2019 Jason A. Donenfeld <Jason@zx2c4.com>
# Copyright (C) 2016 Baptiste Jonglez <openwrt@bitsofnetworks.org>
# Copyright (C) 2016-2017 Dan Luedtke <mail@danrl.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=wireguard-tools

PKG_VERSION:=1.0.20250521
PKG_RELEASE:=1

PKG_SOURCE:=wireguard-tools-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://git.zx2c4.com/wireguard-tools/snapshot/
PKG_MD5SUM:=7d991840fe6e4eb524dd46a994242bba

PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_PARALLEL:=1
PKG_USE_MIPS16:=0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/package-defaults.mk

MAKE_PATH:=src
MAKE_VARS += PLATFORM=linux

define Package/wireguard-tools
  $(call Package/wireguard/Default)
  TITLE:=WireGuard userspace control program (wg)
  DEPENDS:=+libmnl +ip
endef

define Package/wireguard-tools/description
  WireGuard is a novel VPN that runs inside the Linux Kernel and utilizes
  state-of-the-art cryptography. It aims to be faster, simpler, leaner, and
  more useful than IPSec, while avoiding the massive headache. It intends to
  be considerably more performant than OpenVPN.  WireGuard is designed as a
  general purpose VPN for running on embedded interfaces and super computers
  alike, fit for many different circumstances. It uses UDP.

  This package provides the userspace control program for WireGuard and init script.
endef

define Package/wireguard-tools/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/wg-client.init $(1)/etc/init.d/wg-client
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/wg $(1)/usr/bin/
endef

$(eval $(call BuildPackage,wireguard-tools))
