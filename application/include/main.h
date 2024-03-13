#ifndef MAIN_H
#define MAIN_H

// #include "visController.h"
#include <mutex>
#include "visController.h"
#include "visIpcMsg.h"
#include "visControl.h"

//device mode enum
enum visDeviceMode{
    Pause = 0,
    Main = 1,
    Config = 2
};


void helloworld();

#endif