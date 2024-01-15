//TODO: add AI image computation process
#include "main.h"
#include "visIpcMsg.h"

void print_buf(uint8_t * buf, int length){
    for(int i = 0; i < length; i++){
        printf("%x ", buf[i]);
    }
    printf("\n");
    return;
}

int main(){
    visSharedMemory * shm_obj = new visSharedMemory(BUF_LENGTH_FRAME, SHM_KEY_ID);
    int ret = shm_obj->init();
    if(ret == 0){
        printf("visAlgo closed due to invalid shared memory opening error");
        return 0;
    }
    while(1){
        uint8_t * buf = shm_obj->receive();
        if(buf == NULL)
            sleep(1);// not receiving anything
        else{
            //print out the buf
            print_buf(buf, 1024);
        }
    }
    return 0;
}