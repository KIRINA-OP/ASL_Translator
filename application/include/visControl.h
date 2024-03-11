#ifndef VISCONTROL_H
#define VISCONTROL_H


#include <vector>
#include "visConfig.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <mutex>
#include <unistd.h>
#include "gpiod/include/gpiod.h"
#include <unordered_map>


// #include <sys/mman.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

//define specific pins
#define PB0 32 // (B-1) * 32 + 0 = 

#define OLED_CS         8		
#define OLED_RST        PB0	
#define OLED_DC         25	

const unsigned int M_SECOND = 1000;

class visControl{
    protected:
    std::mutex lock;
    public:

};

//TODO: implement during camera control ticket
class visCameraControl: public visControl{
    /*
        visCameraControl class is inheritated from visControl
        responsible for taking photo
        store the information in the shared memory
    */
    visCameraConfig* config;

    public:
    visCameraControl();
    //taking photo
    void captureframe();
    //preprocessing
    void preprocessing();
    //communicate with IPC
    void sendFrame(); //produce frame

    ~visCameraControl();
};

//gpio polling by starting a pthread from thread pool
class visGpioControl: public visControl{
    std::string dev_name;

    gpiod_chip *chip;
    // GPIO_REGISTER * gpio_base;
    std::unordered_map<UWORD, gpiod_line*> pin_read;
    std::unordered_map<UWORD, gpiod_line*> pin_write;
    bool configPin(UWORD pin, bool write);
    public:
    visGpioControl();
    bool init();
    UBYTE readPin(UWORD pin);
    bool writePin(UWORD pin, UBYTE val);
    ~visGpioControl();
};

//TODO: implement in screen control
class visScreenControl: public visControl{
    void oled_reset();
    visGpioControl * gpio;
    public:
    visScreenControl(visGpioControl* gpio);
    bool init();
    void screenClear();
    void paintNewImage();
    void paintSelectImage();
    void disPlay();

    bool acquireLock();
    ~visScreenControl();
};



#endif