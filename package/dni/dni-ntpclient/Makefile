# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.4.3 2007/02/27 03:06:36 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=ntpclient
PKG_RELEASE:=4
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

CONFIG_NTPCLIENT_GIT_TREEISH="ntpclient_2015_365-dni42"
CONFIG_NTPCLIENT_GIT_REPOSITORY="ntpclient.git"

PKG_GIT_TREEISH:=$(CONFIG_NTPCLIENT_GIT_TREEISH)

LOCAL_PKG_NAME_EXT:=dni-ntpclient

include $(INCLUDE_DIR)/package.mk

define Package/dni-ntpclient
  SECTION:=utils
  CATEGORY:=Base system
  DEPENDS:=@TARGET_ipq806x +datalib +librt
  TITLE:=The CGI interface
endef

define Package/dni-ntpclient/description
	The CGI interface
endef


TARGET_CFLAGS += -DDGC_IF_WAN_IFNAME="\\\"$(call qstrip,$(CONFIG_DGC_IF_WAN_IFNAME))\\\"" \
                 -DDGC_IF_PPP_IFNAME="\\\"$(call qstrip,$(CONFIG_DGC_IF_PPP_IFNAME))\\\"" \
		 -DDGC_IF_MOBILE_WAN_IFNAME="\\\"$(call qstrip,$(CONFIG_DGC_IF_MOBILE_WAN_IFNAME))\\\""

define Build/Prepare
	$(CP) $(GIT_HOME)/ntpclient.git/* $(PKG_BUILD_DIR)/
endef

define Package/dni-ntpclient/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/src/ntpclient $(1)/usr/sbin/
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/ntpclient.init $(1)/etc/init.d/ntpclient
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,dni-ntpclient))
