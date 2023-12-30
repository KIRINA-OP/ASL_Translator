#dd flash u-boot if user assigned to do so do it

copy_target_rootfs(){
    echo "moving target rootfs to tf card"
    tar -cf target-rootfs.tar emdebian/mindb/target-rootfs/
    tar -xf target-rootfs.tar -C /media/stephen/rootfs
    mv /media/stephen/rootfs/emdebian/mindb/target-rootfs/* /media/stephen/rootfs/
    rm -rf /media/stephen/rootfs/emdebian
    rm target-rootfs.tar
    echo "success"
}

#flash linux kernel, same as that one
#once the user has the linux kernel compiled

#1. get the boot.cmd compiled and flash
kernelPath="/media/stephen/KERNEL/" #change to target kernel path on tf card
kernelImage="./linux/arch/arm/boot/zImage"
bootcmd_path="u-boot/boot.cmd"
bootscr_path="u-boot/boot.scr"
rootfsPath="/media/stephen/rootfs" #change to target rootfs on tf card
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
if [ ! -d "./emdebian/mindb/target-rootfs/" ]; then 
    echo "cannot find valid target-rootfs"
    exit -1
fi

#copy the compiled binary file to the target debian

appPathDst="./emdebian/mindb/target-rootfs/usr/sbin/visapp"
appPathSrc="./application/build/bin/visapp"
if [ -e $appPathDst ] && [ -e $appPathSrc ]; then
    #check the last modify time
    dst_time=$(stat -c %Y $appPathDst)
    src_time=$(stat -c %Y $appPathSrc)
    if [ $src_time -gt $dst_time ]; then
        echo "copying the source application binary file to the target rootfs"
        cp $appPathSrc $appPathDst
        echo "copied success"
    else
        echo "no updates to the source application binary"
    fi
else
    if [ -e $appPathSrc ]; then
        #directly copy to the destination path
        echo "copying the source application binary file to the target rootfs"
        cp $appPathSrc $appPathDst
        echo "copied success"
    else
        echo "firmware application doesn't exist"
    fi
fi

if [ -z "$(ls -A $rootfsPath)" ]; then
    #empty rootfs, then copy without any hesitate
    copy_target_rootfs
else 
    if [ ! $# -eq 0 ] && [ "$1" = "-f" ]; then
        #remove the current rootfs on tf card
        rm -rf $rootfsPath/*
        copy_target_rootfs
    else
        echo "do not cover the existing target rootfs on tf card"
    fi
    #check for arguments and else
fi


