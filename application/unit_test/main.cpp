#include "main.h"
#include "unitTest.h"


void test_gpio(){
    visGpioControl* visGpio = new visGpioControl();
    visGpio->init();
    visGpio->writePin(PB0, (uint8_t)1);
    usleep(500 * M_SECOND);
    std::cout<<"read val: "<<std::to_string(visGpio->readPin(PB0))<<std::endl;
    usleep(500 * M_SECOND);
    visGpio->writePin(PB0, (uint8_t)0);

    std::cout<<"pb0 test success"<<std::endl;
    delete visGpio;
    visGpio = NULL;
}

int main(){
    test_oled();
    return 0;
}