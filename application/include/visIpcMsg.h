#ifndef VISIPCMSG_H
#define VISIPCMSG_H

class visIpcMsg{
public:
    visIpcMsg();
    ~visIpcMsg();
};

class visGuiMessageQueue: public visIpcMsg{

};

class visSharedMemory: public visIpcMsg{

};

//TODO AI Integration: considering the response from the AI process, 
//considering add a messageQueue or a Pipe for this part of communication 
#endif