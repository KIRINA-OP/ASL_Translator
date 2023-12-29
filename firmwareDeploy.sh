#dd flash u-boot if user assigned to do so do it



#flash linux kernel, same as that one
#once the user has the linux kernel compiled

#1. get the boot.cmd compiled and flash
kernelPath="/media/stephen/KERNEL/"
kernelImage="./linux/arch/arm/boot/zImage"
bootcmd_path="u-boot/boot.cmd"
bootscr_path="u-boot/boot.scr"
if [ -e $bootcmd_path ]; then
    mkimage -C none -A arm -T script -d $bootcmd_path $bootscr_path
    cp $bootscr_path $kernelPath
    echo "boot_cmd file has been copied successfully to $kernelPath"
else
    echo "cannot find $bootcmd_path."
    exit -1
fi

if [ -e $kernelImage ]; then
    # Copy the file to the destination path
    cp $kernelImage $kernelPath
    echo "Linux Kernel image copied successfully to $kernelPath"
else
    echo "cannot find $kernelImage."
    exit -1
fi

#check for dtb file existing
dtbFile="./linux/arch/arm/boot/dts/sun8i-v3s-licheepi-zero.dtb"
if [ -e $dtbFile ]; then
    cp $dtbFile $kernelPath
    echo "dtb file copied successfully to $kernelPath"
else
    echo "cannot find $dtbFile."
    exit -1
fi

#tar the emdebian file and flash it to the tf card
#check for target-rootfs directory exists or not
if [ ! -d "./emdebian/mindb/target-rootfs/"]; then 
    echo "cannot find valid target-rootfs"
    exit -1
fi

#copy the init file to the init.d

tar -cvf target-rootfs.tar emdebian/mindb/target-rootfs/
tar -xvf target-rootfs.tar -C /media/stephen/rootfs
mv /media/stephen/rootfs/emdebian/mindb/target-rootfs/* /media/stephen/rootfs/
rm -rf /media/stephen/rootfs/emdebian
rm target-rootfs.tar