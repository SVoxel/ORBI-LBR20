CONFIG_QCA_SINGLE_IMG_TREEISH = 846f519b1177337670c79bc5d0b497d7b2f4440e

export CONFIG_QCA_SINGLE_IMG_TREEISH

single_img_dep = $(obj)u-boot

define BuildSingleImg
	cp $(obj)u-boot openwrt-ipq40xx-u-boot-stripped.elf
	sstrip openwrt-ipq40xx-u-boot-stripped.elf

	board/"$(BOARDDIR)"/gen-single-img.sh --force-remove \
			--git-repo "$(CONFIG_QCA_SINGLE_IMG_GIT)" \
			--treeish $(CONFIG_QCA_SINGLE_IMG_TREEISH) \
			-w "tools/qca_single_img/qsdk-chipcode" \
			-o . \
			openwrt-ipq40xx-u-boot-stripped.elf

endef
