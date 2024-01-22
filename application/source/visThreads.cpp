#include "visThreads.h"
#include "visIpcMsg.h"


//this is the test function
void getFrameBuf(uint8_t * buf, int len){
    for(int i = 0; i < len; i++){
        buf[i] = i;
    }
    return;
}

void print_buf(uint8_t * buf, int len){
    for(int i = 0; i < len; i++){
        printf("%x ", buf[i]);
    }
    printf("end \n");
}

void frameTransfer(){
    visSocketApp app_socket = visSocketApp(APP_SOCKET_PATH, ALGO_SOCKET_PATH);
    app_socket.init();
    uint8_t * deliver_buf = new uint8_t[BUF_LENGTH_FRAME];
    //start two threads one for sending and one for receiving
    getFrameBuf(deliver_buf, BUF_LENGTH_FRAME);
    app_socket.deliver(deliver_buf, BUF_LENGTH_FRAME);
    sleep(1);
    uint8_t * receive_buf = new uint8_t[MAXLINE];
    app_socket.receive(receive_buf, MAXLINE);
    printf("app_received: ");
    print_buf(receive_buf, MAXLINE);
    delete[] deliver_buf;
    deliver_buf = NULL;
    delete[] receive_buf;
    receive_buf = NULL;
    return;
}