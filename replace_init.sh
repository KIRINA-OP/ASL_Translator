initPath="./application/init_script"
targetrfPath="./emdebian/mindb/target-rootfs"

replace_visapp_init(){
    #copy the visapp to it 
    if [ -e "$targetrfPath/etc/init.d/visapp" ]; then
        rm $targetrfPath/etc/init.d/visapp
    fi
    cp "$initPath/visapp" "$targetrfPath/etc/init.d/visapp"
    #chroot and enable the visapp init
    chmod a+rx $targetrfPath/etc/init.d/visapp
    LC_ALL=C LANGUAGE=C LANG=C chroot $targetrfPath update-rc.d visapp defaults
}

replace_all_init(){
    mount -o bind /dev/ $targetrfPath/dev/
    replace_visapp_init
    umount $targetrfPath/dev/
}

if [ $# -eq 0 ] || [ $1 = "-al" ]; then
    #add replace all
    echo "replace all"
    replace_all_init
    exit 0
fi


if [ $1 = "-h" ]; then
    #help
    echo " help message for argument: \n -a to replace the visapp init file on TF card \n -al to repalce all the required init files"
elif [ $1 = "-a" ]; then
    # according to the emDebian 
    mount -o bind /dev/ $targetrfPath/dev/
    replace_visapp_init
    umount $targetrfPath/dev/
else
    echo "invalid command argument, type in -h to have more information"
fi #love u muamua by miaomiao