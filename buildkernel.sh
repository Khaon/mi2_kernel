#!/bin/sh

# Colorize and add text parameters
red=$(tput setaf 1) # red
grn=$(tput setaf 2) # green
cya=$(tput setaf 6) # cyan
txtbld=$(tput bold) # Bold
bldred=${txtbld}$(tput setaf 1) # red
bldgrn=${txtbld}$(tput setaf 2) # green
bldblu=${txtbld}$(tput setaf 4) # blue
bldcya=${txtbld}$(tput setaf 6) # cyan
txtrst=$(tput sgr0) # Reset

export KERNELDIR=`readlink -f .`
export PARENT_DIR=`readlink -f ..`
export INITRAMFS_SOURCE=/home/khaon/Documents/kernels/Ramdisks/AOSP_ARIES
export PACKAGEDIR=/home/khaon/Documents/kernels/Packages
export META_INF=/home/khaon/kernels/zip_builders/Aries
export ANY_KERNEL=/home/khaon/kernels/AnyKernel2
#Enable FIPS mode
export USE_SEC_FIPS_MODE=true
export ARCH=arm
export CROSS_COMPILE=/home/khaon/Documents/Toolchains/arm-cortex_a15-linux-gnueabihf-linaro_4.9.3-2015.03/bin/arm-cortex_a15-linux-gnueabihf-

echo "${txtbld} Remove old zImage ${txtrst}"
make mrproper
rm arch/arm/boot/zImage

echo "${bldblu} Make the kernel ${txtrst}"
make khaon_aries_defconfig

make -j9

if [ -e $KERNELDIR/arch/arm/boot/zImage ]; then
	echo " ${bldgrn} Kernel built !! ${txtrst}"
	echo "Making any kernel flashable zip"

	export curdate=`date "+%m-%d-%Y"`

	rm $PACKAGEDIR/UPDATE-AnyKernel2-khaon-kernel-aries-lollipop*.zip

	cd $ANY_KERNEL;
	git reset --hard;git clean -fdx;git checkout aries;
	cp $KERNELDIR/arch/arm/boot/zImage zImage

  zip -r9 $PACKAGEDIR/UPDATE-AnyKernel2-khaon-kernel-aries-lollipop-"${curdate}".zip * -x README UPDATE-AnyKernel2.zip .git *~

	cd $KERNELDIR
else
	echo "KERNEL DID NOT BUILD! no zImage exist"
fi;

