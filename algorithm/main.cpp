//TODO: add AI image computation process
#include "main.h"
#include "visIpcMsg.h"


int main(){
    visSharedMemory * shm_obj;
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
        }
    }

}