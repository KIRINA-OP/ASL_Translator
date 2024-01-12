#ifndef VISCONTROLLER_H
#define VISCONTROLLER_H

class visDeviceControl;
class visCameraControl;
class visController{
    //listen to the request from the GUI process and handle it
    // visTaskQueue* requests;
    visDeviceControl* device;
    visCameraControl* camera;
    //this is usually taken from the user input to the device

    //user input -> msg to gui -> gui response -> perform controlling through viscontroller
    public:
    visController();
    bool acquireLock();
    ~visController();
};
#endif