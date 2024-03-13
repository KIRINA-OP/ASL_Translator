#ifndef VISCONFIG_H
#define VISCONFIG_H
#include <mutex>
#include <string>
#include <stdexcept>


//TODO: config implementation can be left later 
class visConfig{
    protected:
    std::mutex v_lock;
    std::string xml_file_path;
    public:
    visConfig();
    bool acquireLock();
    bool releaseLock();
    virtual void readConfig();
    virtual void writeConfig();
    virtual void writeToDevice() = 0;
    ~visConfig();
};

class visFontConfig: public visConfig{
    size_t font_type;
    size_t font_size;
    public:
    visFontConfig();
    void readConfig();
    void writeConfig();
    void writeToDevice();
    ~visFontConfig();
};

class visDisplayConfig: public visConfig{
    visFontConfig * font_config;
    int display_resolution;
    public:
    //contain the information to control the spi/i2c screen
    visDisplayConfig();
    void readConfig();
    void writeConfig();
    void writeToDevice();
    ~visDisplayConfig();
};



class visCameraConfig: public visConfig{
    int framePerSecond;
    int cameraResolution;

    public:
    visCameraConfig();
    void readConfig();
    void writeConfig();
    void writeToDevice();
    ~visCameraConfig();
};


#endif