Copyright (C) 2007-2009 Delta Networks Inc.

Small application for saving data on embedded system. It runs as
multi-clients & server mode with socket-call.

When you check this source code from GIT tree, please modify as bellowing
for meeting with your specific project.

1). ** Different project should has different marco definitions:

--1.1> Modify or add following message in package/datalib/Makefile line 26, similar with following:
#
# Please modify following variable to meet with your specific project
#
# CONFIG_MAGIC          "The Unique ID of the configuration file, such as your project started data: 0x20090330"
# RAND_MAGIC            "The secret key, it must be different with CONFIG_MAGIC. such as 0x23091293"
# PATH_CONFIG_MTD       "Where the MTD flash in your DUT, such as '/dev/mtd/3'"
# 
CONFIG_MAGIC=0x20090330
RAND_MAGIC=0x23091293
PATH_CONFIG_MTD=\\\"/dev/mtd/3\\\"

--1.2>Modify package/datalib/Makefile if necessary:

CONFIG_MAGIC="$(CONFIG_MAGIC)" \
RAND_MAGIC="$(RAND_MAGIC)" \
PATH_CONFIG_MTD="$(PATH_CONFIG_MTD)" \

Add above after about line 55:
	LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib" \


2). Over-write the code file 'defaults.c':
	Add your specific project name/value configuration pairs.


It SHOULD be easily understandable. ^_^

Regards,
Haiyue @ 2009-03-30

=====================================================================================================================

Example of package/datalib/Makefile:


#
# Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

REGION_TAG=REGION_WW
ifeq ($(FW_REGION),NA)
  REGION_TAG=REGION_NA
endif
ifeq ($(FW_REGION),GR)
  REGION_TAG=REGION_GR
endif
ifeq ($(FW_REGION),PR)
  REGION_TAG=REGION_PR
endif
ifeq ($(FW_REGION),KO)
  REGION_TAG=REGION_KO
endif

PKG_NAME:=datalib
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_GIT_TREEISH:=548991cc7901d7940af031be4f601cedbe86dc24

# 
# Please modify following variable to meet with your specific project
# 
# CONFIG_MAGIC		"The Unique ID of the configuration file, such as your project started data: 0x20090330"
# RAND_MAGIC		"The secret key, it must be different with CONFIG_MAGIC. such as 0x23091293"
# PATH_CONFIG_MTD	"Where the MTD flash in your DUT, such as '/dev/mtd/3'"
#
CONFIG_MAGIC=0x20120830
RAND_MAGIC=0x23091293
PATH_CONFIG_MTD=\\\"/dev/mtd5\\\"

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/datalib
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_dniwndr3700u2631||TARGET_wndr4300
  TITLE:=Small application for saving data on embedded system
endef

define Build/Prepare
	test x$(GIT_HOME) != x
	test -d $(GIT_HOME)/datalib.git
	(cd $(GIT_HOME)/datalib.git; git-cat-file -e $(PKG_GIT_TREEISH))
	(cd $(BUILD_DIR); git-archive --format=tar --prefix=$(PKG_NAME)/ --remote=$(GIT_HOME)/datalib.git $(PKG_GIT_TREEISH) | tar -xvf -)
	$(call Build/Patch/Default)
	$(CP) ./files/defaults.c $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
                $(TARGET_CONFIGURE_OPTS) \
                CROSS="$(TARGET_CROSS)" \
		CC="$(TARGET_CROSS)gcc" \
                CFLAGS="$(TARGET_CFLAGS) -D$(REGION_TAG) -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include" \
		CFLAGS+="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include" \
                LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib" \
		CONFIG_MAGIC="$(CONFIG_MAGIC)" \
		RAND_MAGIC="$(RAND_MAGIC)" \
		PATH_CONFIG_MTD="$(PATH_CONFIG_MTD)" \
                all
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(STAGING_DIR)/usr/lib/
endef

define Build/UninstallDev
	rm -f $(STAGING_DIR)/usr/lib/libconfig.so
endef

define Package/datalib/install
	install -d -m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(1)/usr/lib/
	install -d -m0755 $(1)/bin
	install -m0755 $(PKG_BUILD_DIR)/datalib $(1)/bin/
	install -m0755 $(PKG_BUILD_DIR)/config $(1)/bin/
endef

$(eval $(call BuildPackage,datalib))
