#ifndef VISIPCMSG_H
#define VISIPCMSG_H

class visIpcMsg{
public:
    visIpcMsg();
    void readMsg();
    void writeMsg();
    ~visIpcMsg();
};

class visGuiMessageQueue: public visIpcMsg{

};

class visFrame{

};
class visSharedMemory: public visIpcMsg{
    //send Frame to AI process
    void ** buf; //frame buffer
    
};

//TODO AI Integration: considering the response from the AI process, 
//considering add a messageQueue or a Pipe for this part of communication 
#endif