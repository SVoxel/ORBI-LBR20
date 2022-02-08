CONFIG_QCA_SINGLE_IMG_TREEISH = 1ad3fcecba37ce02b16ded263fd9fc3229a2e291

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
