#ifndef VISIPCMSG_H
#define VISIPCMSG_H
#include "main.h"
#include <unistd.h>
#include <sys/shm.h>
class visIpcMsg{

};

class visGuiMessageQueue: public visIpcMsg{

};


struct visFrame{
    int length;
    int width;
    //other pointers given by opencv
};
struct visShmMsg{
    int shmSig; // the signal here behave like a lock, when new image transferring, it will be set as 1, until the other side got the full image, the signal will be set back to 0
    void* chBuffer;
};

class visSharedMemory: public visIpcMsg{
    //send Frame to AI process
    //this is only for read and write


    visShmMsg * shm_msg;
    int shm_id;
    int buf_length;
    inline int getShSig(){return shm_msg->shmSig;};

    public:
    visSharedMemory(int l);
    int init();
    inline int getId(){return shm_id;};
    int deliver(uint8_t * content);
    uint8_t * receive();
    ~visSharedMemory();    
};

//TODO AI Integration: considering the response from the AI process, 
//considering add a messageQueue or a Pipe for this part of communication 
#endif