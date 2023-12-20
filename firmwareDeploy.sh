#dd flash u-boot if user assigned to do so do it

#flash linux kernel, same as that one

#tar the emdebian file and flash it to the tf card
tar -cvf target-rootfs.tar emdebian/mindb/target-rootfs
tar xvf target-rootfs.tar -C /media/stephen/rootfs