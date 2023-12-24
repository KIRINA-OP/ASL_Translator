#dd flash u-boot if user assigned to do so do it



#flash linux kernel, same as that one
#once the user has the linux kernel compiled

#1. get the boot.cmd compiled and flash
kernelPath="/media/$USER/KERNEL/"
kernelImage="./linux/arch/arm/boot/zImage"
bootcmd_path="u-boot/boot.cmd"
bootscr_path="u-boot/boot.scr"
if [ -e $bootcmd_path ]; then
    mkimage -C none -A arm -T script -d $bootcmd_path $bootscr_path
    cp $bootscr_path $kernelPath
    echo "boot_cmd file has been copied successfully to $kernelPath"
else
    echo "cannot find $bootcmd_path."
fi

if [ -e $kernelImage ]; then
    # Copy the file to the destination path
    cp $kernelImage $kernelPath
    echo "Linux Kernel image copied successfully to $kernelPath"
else
    echo "cannot find $kernelImage."
fi


#tar the emdebian file and flash it to the tf card
tar -cvf target-rootfs.tar emdebian/mindb/target-rootfs/
tar -xvf target-rootfs.tar -C /media/$USER/rootfs
mv /media/$USER/rootfs/emdebian/mindb/target-rootfs/* /media/$USER/rootfs/
rm -rf /media/$USER/rootfs/emdebian
rm target-rootfs.tar