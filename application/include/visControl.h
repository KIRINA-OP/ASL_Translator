#ifndef VISCONTROL_H
#define VISCONTROL_H


#include <cstdint>
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
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


// #include <sys/mman.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define RAS_PI 1

#ifndef RAS_PI
//define specific pins
#define PB0 32 // (B-1) * 32 + 0 = 
#define PE21 149
#define PE22 150
//define other pins used for CS and DC

#define OLED_CS         PE21		
#define OLED_RST        PB0	
#define OLED_DC         PE22	

const UWORD BLACK = 0x0000;
const UWORD WHITE = 0xFFFF;
const uint8_t I2C_CMD = 0x00;
const uint8_t I2C_RAM = 0x40;

const std::string I2C_FILE = "/dev/i2c-0";
const UBYTE IMAGE_SIZE = (UBYTE)2048;
const UBYTE IMAGE_HEIGHT = 128;
const UBYTE IMAGE_WIDTH = 64;
#endif

const unsigned int M_SECOND = 1000;
const uint32_t SCREEN_ADDR = 0x3c;


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

class visI2CControl: public visControl{

    std::string dev_name;
    uint8_t addr;
    int fd;
    public:
    visI2CControl(uint8_t a);
    bool init();
    bool write_i2c(uint8_t ad, uint8_t data);
    ~visI2CControl();
};

struct visPaint{
    UBYTE *Image;
    UWORD Width;
    UWORD Height;
    UWORD WidthMemory;
    UWORD HeightMemory;
    UWORD Color;
    UWORD Rotate;
    UWORD Mirror;
    UWORD WidthByte;
    UWORD HeightByte;
    UWORD Scale;
} ;

//TODO: implement in screen control
class visScreenControl: public visControl{
    void oledReset();
    visGpioControl * gpio;
    visI2CControl * i2c_obj;
    void oledWriteReg(uint8_t reg);
    void oledWriteData(uint8_t val);
    UBYTE* image;
    UWORD rotate;
    public:
    visScreenControl(visGpioControl* gpio, visI2CControl* i2c);
    bool init();
    void screenClear();
    void paintDrawString(std::string text, int line);
    void paintSelectImage();
    void paintClear(UWORD color);
    
    void paintNewImage(UWORD rotate, UWORD color);
    void oledDisplay();
    bool acquireLock();
    ~visScreenControl();
};



#endif