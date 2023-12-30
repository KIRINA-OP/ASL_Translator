initPath="./application/init_script"
targetrfPath="./emdebian/mindb/target-rootfs/etc/init.d"

replace_visapp_init(){
    #copy the visapp to it 
    cp "$initPath/visapp" "$targetrfPath/visapp"
    #chroot and enable the visapp init
}

replace_all_init(){
    replace_visapp_init
}

if [ $# -eq 0 ] || [ $1 = "-al" ]; then
    #add replace all
    echo "replace all"
    replace_all_init
    exit 0
fi


if [ $1 = "-h" ]; then
    #help
elif [ $1 = "-a" ]; then
    #replace visapp
else
    echo "invalid command argument"
fi #love u muamua by miaomiao