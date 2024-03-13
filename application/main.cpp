#include "main.h"
#include "visControl.h"
#include <string>


#include "visIpcMsg.h"
#include "visThreads.h"
#ifndef RAS_PI
visScreenControl* screenInit(visGpioControl* gpio, visI2CControl* i2c){
    //init the gpio struct
    visScreenControl* ret = new visScreenControl();
    ret->init();
    //init the screen control
    ret->screenClear();
    //init the camera
    ret->paintClear(WHITE);
    ret->oledDisplay();
    return ret;
}
#endif

visScreenControl* screenInit(){
    visScreenControl* ret = new visScreenControl(); 
    if(ret->init()){
        return ret;
    }
    return NULL;
}

int main(){
	visScreenControl* screen_obj = screenInit();
    if(screen_obj == NULL){
        std::cout<<"screen start failed"<<std::endl;
        return -1;
    }
    visSocketApp app_sock = visSocketApp(APP_SOCKET_PATH, ALGO_SOCKET_PATH);
    app_sock.init();
    std::vector<std::thread> workers;
    workers.emplace_back(std::thread(receiveText, app_sock, screen_obj));
    workers.emplace_back(std::thread(displayOutput, screen_obj));
    for(int i = 0; i < workers.size(); i++){
        workers[i].join();
    }
    //delete screen object
    delete screen_obj;
    screen_obj = NULL; 
    return 0;
}

