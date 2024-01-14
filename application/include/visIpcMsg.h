#ifndef VISIPCMSG_H
#define VISIPCMSG_H
#include "main.h"
#include <unistd.h>
#include <sys/shm.h>
class visIpcMsg{
public:
    visIpcMsg();
    ~visIpcMsg();
};

class visGuiMessageQueue: public visIpcMsg{

};


struct visFrame{
    int length;
    int width;
    //other pointers given by opencv
};
struct visShmMsg{
    int iSignal;
    void* chBuffer;
};

class visSharedMemory: public visIpcMsg{
    //send Frame to AI process
    //this is only for read and write


    visShmMsg * shm_msg;
    int shm_id;
    int buf_length;
    public:
    visSharedMemory(int l);
    int init();
    int get_id();
    void deliver();
    ~visSharedMemory();    
    
};

//TODO AI Integration: considering the response from the AI process, 
//considering add a messageQueue or a Pipe for this part of communication 
#endif