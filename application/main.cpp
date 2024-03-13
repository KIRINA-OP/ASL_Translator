#include "main.h"
#include "visControl.h"
#include <string>
// #include "visThreads.h"

visScreenControl* screenInit(visGpioControl* gpio, visI2CControl* i2c){
    //init the gpio struct
    visScreenControl* ret = new visScreenControl(gpio, i2c);
    ret->init();
    //init the screen control
    ret->screenClear();
    //init the camera
    ret->paintClear(WHITE);
    ret->oledDisplay();
    usleep(1000*M_SECOND);
    return ret;
}




int main(){
    visGpioControl * gpio = new visGpioControl();
    gpio->init();
    // frameTransfer();
    visI2CControl* i2c_obj = new visI2CControl(SCREEN_ADDR);
    i2c_obj->init();
    auto screen = screenInit(gpio, i2c_obj);
    delete screen;
    screen = NULL;
    delete gpio;
    gpio = NULL;
    delete i2c_obj;
    i2c_obj = NULL;
    return 0;
}