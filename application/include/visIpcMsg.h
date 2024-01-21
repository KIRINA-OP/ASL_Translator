#ifndef VISIPCMSG_H
#define VISIPCMSG_H
#include "main.h"
#include <unistd.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <cstddef>

class visIpcMsg{

};

class visGuiMessageQueue: public visIpcMsg{

};


struct visFrame{
    int length;
    int width;
    //other pointers given by opencv
};


const int SHM_KEY_ID = 1234;
const int BUF_LENGTH_FRAME = 1024;
class visSocket: public visIpcMsg{
    //send Frame to AI process
    //this is only for read and write
    protected:
    std::string app_sock_path;
    std::string algo_sock_path;
    struct sockaddr_un appun;
    struct sockaddr_un algoun;
    public:
    virtual int init() = 0;
    virtual int deliver(uint8_t * content, int len) = 0;
    virtual int receive(uint8_t * buf, size_t len) = 0;
};

const int MAXLINE = 100;
class visSocketApp: protected visSocket{
    int app_sock;
    public:
    visSocketApp(std::string app_path, std::string algo_path);
    int init();
    int deliver(uint8_t * content, int len);
    int receive(uint8_t * buf, size_t len);
    ~visSocketApp();
};

class visSocketAlgo: protected visSocket{
    int algo_sock;
    socklen_t appun_len;
    int app_sock;
    public:
    visSocketAlgo(std::string app_path, std::string algo_path);
    int init(); 
    int deliver(uint8_t * content, int len);
    int receive(uint8_t * buf, size_t len);
    ~visSocketAlgo();

};

//TODO AI Integration: considering the response from the AI process, 
//considering add a messageQueue or a Pipe for this part of communication 
#endif