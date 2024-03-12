//TODO: add AI image computation process
#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <iostream>
#include "visIpcMsg.h"
#include "visThreads.h"

int main(int argc, char **argv){
    visSocketAlgo algo_socket(APP_SOCKET_PATH, ALGO_SOCKET_PATH);
    algo_socket.init();
    // uint8_t* receive_buf = new uint8_t[BUF_LENGTH_FRAME];
    // uint8_t* deliver_buf = new uint8_t[MAXLINE];
    std::string imagePath = "/home/matts/fydp/ASL_Translator/test.png";
    std::string receivedImagePath = "/home/matts/fydp/ASL_Translator/out_test.png";
    while(1){
        int n = algo_socket.receive(imagePath);
        if(n == 1)
            break;
        else if(n == -1){
            break;
        }
        else if (n == 0){
            algo_socket.deliver(receivedImagePath);
        }
    }
    // delete[] receive_buf;
    // delete[] deliver_buf;
    // receive_buf = NULL;
    // deliver_buf = NULL;
    return 0;
}