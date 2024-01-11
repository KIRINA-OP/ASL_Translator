#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <string>


#define LOCK pthread_mutex_t 

//device mode enum
enum visDeviceMode{
    Pause = 0,
    Main = 1,
    Config = 2
};
void helloworld();

#endif