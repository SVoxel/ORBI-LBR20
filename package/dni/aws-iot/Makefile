#
# Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=aws-iot
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

CONFIG_AWS_IOT_GIT_TREEISH:="fe9ea0099d520cc9a3cc7d8b8ef6c34d0c45bf9d"
CONFIG_AWS_IOT_GIT_REPOSITORY:="aws-iot.git"

PKG_GIT_TREEISH:=$(CONFIG_AWS_IOT_GIT_TREEISH)

LOCAL_PKG_NAME_EXT:=aws-iot

include $(INCLUDE_DIR)/package.mk

define Package/aws-iot
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+datalib +libcurl
  TITLE:=The AWS IoT to Publish Topic
endef

EXTRA_LDFLAGS := $(EXTRA_CFLAGS) $(call qstrip,$(CONFIG_AWSIOT_EXTRA_FLAGS)) 

define Package/aws-iot/config
	menu "Configuration"
	depends on PACKAGE_dniacl
	source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/aws-iot.git/* $(PKG_BUILD_DIR)/
	echo "WAN_IFNAME=\\\"$(CONFIG_DGC_IF_WAN_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "PPP_IFNAME=\\\"$(CONFIG_DGC_IF_PPP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "LAN_IFNAME=\\\"$(CONFIG_DGC_IF_LAN_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "MAIN_2G=\\\"$(CONFIG_DGC_IF_WLAN_Base_2G_AP_IFNAME)\\\"" >>  $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "MAIN_5G=\\\"$(CONFIG_DGC_IF_WLAN_Base_5G_AP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "GUEST_2G=\\\"$(CONFIG_DGC_IF_WLAN_Base_2G_GUEST_AP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "GUEST_5G=\\\"$(CONFIG_DGC_IF_WLAN_Base_5G_GUEST_AP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "BACKHAUL_2G=\\\"$(CONFIG_DGC_IF_WLAN_Base_2G_BH_AP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
	echo "BACKHAUL_5G=\\\"$(CONFIG_DGC_IF_WLAN_Base_5G_BH_AP_IFNAME)\\\"" >> $(PKG_BUILD_DIR)/files/aws-iot.config
endef

define Build/Compile
endef

define Package/aws-iot/install
	install -d -m0755 $(1)/usr/sbin
	install -d -m0755 $(1)/sbin
	install -d -m0755 $(1)/etc/init.d
	install -d -m0755 $(1)/etc/router_analytics
	install -d -m0755 $(1)/etc/router_analytics/beta
	install -d -m0755 $(1)/etc/router_analytics/debug
	install -d -m0755 $(1)/etc/rc.d
	install -m0755 $(PKG_BUILD_DIR)/files/orbi-aws-iot.init $(1)/etc/init.d/aws
	install -m0755 $(PKG_BUILD_DIR)/files/certs/certificate.pem.crt  $(1)/etc/router_analytics/
	install -m0755 $(PKG_BUILD_DIR)/files/certs/rootCA.crt $(1)/etc/router_analytics
	install -m0755 $(PKG_BUILD_DIR)/files/certs/beta/certificate.pem.crt  $(1)/etc/router_analytics/beta/
	install -m0755 $(PKG_BUILD_DIR)/files/certs/beta/rootCA.crt $(1)/etc/router_analytics/beta/
	install -m0755 $(PKG_BUILD_DIR)/files/certs/debug/certificate.pem.crt  $(1)/etc/router_analytics/debug/
	install -m0755 $(PKG_BUILD_DIR)/files/certs/debug/rootCA.crt $(1)/etc/router_analytics/debug/
	install -m0755 $(PKG_BUILD_DIR)/files/ping-gateway $(1)/sbin/ping-gateway
	install -m0755 $(PKG_BUILD_DIR)/files/collect_log.sh $(1)/sbin/collect_log.sh
	install -m0755 $(PKG_BUILD_DIR)/ra_check  $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/files/ra_daemon_monitor.sh $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/files/get_wireddevice_linkspeed.sh  $(1)/usr/sbin
	ln -sv ../init.d/aws $(1)/etc/rc.d/S99aws
	$(call Dni_Install_Ext,$(LOCAL_PKG_NAME_EXT),$(1))
endef

$(eval $(call BuildPackage,aws-iot))
