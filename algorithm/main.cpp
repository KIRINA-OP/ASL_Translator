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
#include <jpeglib.h>

int main(int argc, char **argv){
    visSocketAlgo algo_socket(APP_SOCKET_PATH, ALGO_SOCKET_PATH);
    algo_socket.init();
    JSAMPLE* receive_buf = new uint8_t[BUF_LENGTH_FRAME];
    JSAMPLE* deliver_buf = new uint8_t[MAXLINE];
    while(1){
        int n = algo_socket.receive(receive_buf, BUF_LENGTH_FRAME);
        if(n == 1)
            break;
        else if(n == -1){
            break;
        }
        else if (n == 0){
            getFrameBuf(deliver_buf, BUF_LENGTH_FRAME);
            algo_socket.deliver(deliver_buf, MAXLINE);
        }
    }
    delete[] receive_buf;
    delete[] deliver_buf;
    receive_buf = NULL;
    deliver_buf = NULL;
    return 0;
}