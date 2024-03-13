#ifndef VISGUI_H
#define VISGUI_H
// #include "main.h"
#include "visConfig.h"
#include "visIpcMsg.h"
#include "visTaskQueue.h"
#include "visController.h"

class visGui{
    //this class is responsible for communicating with GUI(QT) through message queue
    std::mutex g_lock;
    visDisplayConfig * display_config;
    visGuiMessageQueue * msg_queue;
    //task queue
    visController* controller;
    public:
    visGui();

    ~visGui();
};


#endif