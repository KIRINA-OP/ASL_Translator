#dd flash u-boot if user assigned to do so do it


copy_target_rootfs(){
    echo "moving target rootfs to tf card"
    rootfsPath="/media/stephen/rootfs"
    target_rootfs_tar="./buildroot-2024.02/output/images/rootfs.tar"
    if [ -e $target_rootfs_tar ]; then
        tar -C $rootfsPath -xf $target_rootfs_tar
        # rm $rootfsPath/rootfs.tar
        echo "complete"
    else
        echo "rootfs doesn't exitst"
        exit -1;
    fi
}

#flash linux kernel, same as that one
#once the user has the linux kernel compiled

rootfsPath="/media/stephen/rootfs"
#1. get the boot.cmd compiled and flash
kernelPath="/media/stephen/KERNEL/" #change to target kernel path on tf card
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

# copy the root file system to the sd card
if [ -z "$(ls -A $rootfsPath)" ]; then
    #empty rootfs, then copy without any hesitate
    copy_target_rootfs
else 
    if [ ! $# -eq 0 ] && [ "$1" = "-f" ]; then
        #remove the current rootfs on tf card
        rm -rf $rootfsPath/*
        echo "deleted the original files"
        copy_target_rootfs
    else
        echo "do not cover the existing target rootfs on tf card"
    fi
fi


#copy the compiled binary file to the target debian

appPathDst="$rootfsPath/usr/sbin/visapp"
appPathSrc="./application/build/bin/visapp"
if [ -e $appPathDst ] && [ -e $appPathSrc ]; then
    #check the last modify time
    dst_time=$(stat -c %Y $appPathDst)
    src_time=$(stat -c %Y $appPathSrc)
    if [ $src_time -gt $dst_time ]; then
        echo "copying the source application binary file to $appPathDst"
        cp $appPathSrc $appPathDst
        echo "copied success"
    else
        echo "no updates to the source application binary"
    fi
else
    if [ -e $appPathSrc ]; then
        #directly copy to the destination path
        echo "copying the source application binary file to $appPathDst"
        cp $appPathSrc $appPathDst
        echo "copied success"
    else
        echo "firmware application doesn't exist"
    fi
fi

algoPathDst="$rootfsPath/usr/sbin/visalgo"
algoPathSrc="./algorithm/build/bin/visalgo"
if [ -e $algoPathDst ] && [ -e $algoPathSrc ]; then
    #check the last modify time
    algo_dst_time=$(stat -c %Y $algoPathDst)
    algo_src_time=$(stat -c %Y $algoPathSrc)
    if [ $algo_src_time -gt $algo_dst_time ]; then
        echo "copying the source algorithm binary file to $algoPathDst"
        cp $algoPathSrc $algoPathDst
        echo "copied success"
    else
        echo "no updates to the source algorithm binary"
    fi
else
    if [ -e $algoPathSrc ]; then
        #directly copy to the destination path
        echo "copying the source application binary file to $algoPathDst"
        cp $algoPathSrc $algoPathDst
        echo "copied success"
    else
        echo "firmware application doesn't exist"
    fi
fi

unitPathSrc="./application/build/bin/unit_test"
unitPathDst="$rootfsPath/usr/sbin/unit_test"
if [ -e $unitPathDst ] && [ -e $unitPathSrc ]; then
    #check the last modify time
    unit_dst_time=$(stat -c %Y $unitPathDst)
    unit_src_time=$(stat -c %Y $unitPathSrc)
    if [ $unit_src_time -gt $unit_dst_time ]; then
        echo "copying the source unit test binary file to $unitPathDst"
        cp $unitPathSrc $unitPathDst
        echo "copied success"
    else
        echo "no updates to the source unit_test binary"
    fi
else
    if [ -e $unitPathSrc ]; then
        #directly copy to the destination path
        echo "copying the source unit test binary file to $unitPathDst"
        cp $unitPathSrc $unitPathDst
        echo "copied success"
    else
        echo "firmware unit test doesn't exist"
    fi
fi

#copy the script
# echo "copying the start script to the target rootfs"
# cp "./start.sh" "./emdebian/mindb/target-rootfs/usr/sbin/start.sh"
# echo "copied"

#copy the driver to the target rootfs
modulePath="./linux/out/lib/modules/*"
rfModulePath="$rootfsPath/lib/modules/"
if [ -e $modulePath ] && [ "$1" = "-f" ]; then
    cp -r $modulePath $rfModulePath
    echo "module files copied successfully to $rfModulePath"
else
    echo "do not copy the module files"
fi