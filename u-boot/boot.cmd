setenv bootm_boot_mode sec
load mmc 0:1 0x41000000 zImage
load mmc 0:1 0x41800000 sun8i-v3s-licheepi-zero.dtb
 
setenv bootargs console=ttyS0,115200 panic=5 rootwait root=/dev/mmcblk0p2 rw vt.global_cursor_default=0
bootz 0x41000000 - 0x41800000