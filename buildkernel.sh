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
export ANY_KERNEL=/media/khaon/54e4d854-fd7a-45a4-ac6b-3b46be763cc92/Documents/kernels/AnyKernel2
export PACKAGEDIR=/home/khaon/Documents/kernels/Packages
export ARCH=arm
export CROSS_COMPILE=/media/khaon/54e4d854-fd7a-45a4-ac6b-3b46be763cc92/android/optiPop/prebuilts/gcc/linux-x86/arm/arm-eabi-4.9/bin/arm-eabi-

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

