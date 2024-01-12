#ifndef VISCONTROL_H
#define VISCONTROL_H
#include "main.h"
#include <vector>
#include "visGui.h"
#include "visConfig.h"
class visControl{
    protected:
    
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
class visDeviceControl: public visControl{
    //gpio Control object
    //guiMessageQueue  
    class GPIO{};
    std::vector<GPIO*> gpios;
    visGui * gui; //take the controller pointer as the parameter, and add task to it
    public:
    //polling from outside
    visDeviceControl(visGui* gui);
    void readGpio();
    void writeGpio();
    ~visDeviceControl();
};

//TODO: implement in screen control
class visScreenControl: public visControl{
    visGui* gui;
    public:
    visScreenControl(visGui* gui);
    ~visScreenControl();
};

#endif