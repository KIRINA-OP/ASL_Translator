#ifndef VISCONTROL_H
#define VISCONTROL_H
#include "main.h"
#include <vector>
#include "visGui.h"
class visControl{
    protected:
    
    public:

};

class visCameraControl: public visControl{
    /*
        visCameraControl class is inheritated from visControl
        responsible for taking photo
        store the information in the shared memory
    */

    public:

};

//gpio polling by starting a pthread from thread pool
class visDeviceControl: public visControl{
    //gpio Control object
    //guiMessageQueue
    class GPIO{};
    std::vector<GPIO*> gpios;
    visGui * gui;
    public:
    //polling from outside
    visDeviceControl();
    void readGpio();
    void writeGpio();
    ~visDeviceControl();
};

class visScreenControl: public visControl{};

#endif