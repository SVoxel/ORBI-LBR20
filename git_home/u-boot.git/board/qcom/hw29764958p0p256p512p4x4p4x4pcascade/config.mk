CONFIG_QCA_SINGLE_IMG_TREEISH = 477f04c2a0fb3bfa0d9dfdba16e6352cb678e752

export CONFIG_QCA_SINGLE_IMG_TREEISH

single_img_dep = $(obj)u-boot.mbn

define BuildSingleImg
	board/"$(BOARDDIR)"/gen-single-img.sh --force-remove \
			--git-repo "$(CONFIG_QCA_SINGLE_IMG_GIT)" \
			--treeish $(CONFIG_QCA_SINGLE_IMG_TREEISH) \
			-w "tools/qca_single_img/qsdk-chipcode" \
			--export-only

	@ ### Steps described in QSDK release notes ###
	cp $(obj)u-boot.mbn \
			$(obj)tools/qca_single_img/qsdk-chipcode/common/build/$(BOARD)/openwrt-ipq806x-u-boot.mbn
	cd $(obj)tools/qca_single_img/qsdk-chipcode/common/build/$(BOARD)/ && \
	python ../../../apss_proc/out/pack.py \
			-t nand -B -F ubootboardconfig \
			-o u-boot-single.img .

	cp -t $(obj)./ \
			$(obj)tools/qca_single_img/qsdk-chipcode/common/build/$(BOARD)/u-boot-single.img
endef
