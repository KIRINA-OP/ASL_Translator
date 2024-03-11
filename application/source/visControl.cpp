#include "visControl.h"
#include "gpiod/include/gpiod.h"
#include <stdexcept>
#include <string>

//first try: using libgpiod.h in C

visGpioControl::visGpioControl(){
}

bool visGpioControl::init(){
    chip = gpiod_chip_open("/dev/gpiochip0");
    if(!chip){
        throw std::runtime_error("unable to get the gpio controller");
        return false;
    }
    return true;
}

bool visGpioControl::configPin(UWORD pin, bool write){
    static int n = 0;
    if(write){
        if(pin_write.find(pin) != pin_write.end())
            return true;
        if(pin_read.find(pin) != pin_read.end()){
            gpiod_line* prev_line = pin_read[pin];
            gpiod_line_release(prev_line);
            pin_read.erase(pin);
        }
        gpiod_line* target_pin = gpiod_chip_get_line(chip, pin);
        gpiod_line_request_output(target_pin, ("w" + std::to_string(n)).c_str(), 0);
        if(!target_pin){

            return false;
        }
        pin_write[pin] = target_pin;
    }
    else{
        if(pin_read.find(pin) != pin_read.end())
            return true;
        if(pin_write.find(pin) != pin_write.end()){
            gpiod_line* prev_line = pin_write[pin];
            gpiod_line_release(prev_line);
            pin_write.erase(pin);
        }
        gpiod_line* target_pin = gpiod_chip_get_line(chip, pin);
        gpiod_line_request_input(target_pin, ("r" + std::to_string(n)).c_str());
        if(!target_pin)
            return false;
        pin_read[pin] = target_pin;
    }
    return true;
}

UBYTE visGpioControl::readPin(UWORD pin){
    if(pin_read.find(pin) == pin_read.end())
        configPin(pin, false);
    gpiod_line* target_pin = pin_read[pin];
    int ret = gpiod_line_get_value(target_pin);
    if(ret == -1)
        printf("invalid read operation");
    return (UBYTE)ret;
}

bool visGpioControl::writePin(UWORD pin, UBYTE val){
    if(pin_write.find(pin) == pin_write.end())
        configPin(pin, true);
    gpiod_line* target_pin = pin_write[pin];
    int ret = gpiod_line_set_value(target_pin, val);
    if(ret == -1){
        printf("invalid write operation");
        return false;
    }
    return true;
}


visGpioControl::~visGpioControl(){
    gpiod_chip_close(chip);
}


visScreenControl::visScreenControl(visGpioControl * gpio){

}

void visScreenControl::oled_reset(){
    gpio->writePin(OLED_RST, 1); //oled rst, 1
    //delay 100ms
    usleep(100 * M_SECOND);
    gpio->writePin(OLED_RST, 0); //oled rst, 1
    usleep(100 * M_SECOND);
    gpio->writePin(OLED_RST, 1); //oled rst, 1
    usleep(100 * M_SECOND);
}


bool visScreenControl::init(){
    //reset

    //init register

    //turn_on writeReg
}