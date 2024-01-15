#include "visThreads.h"
#include "visIpcMsg.h"


void frameTransfer(){
    visSharedMemory * shm_obj = new visSharedMemory(BUF_LENGTH_FRAME, SHM_KEY_ID);
    int ret = shm_obj->init();
    if(ret == 0)
        return;
    //it is for testing
    uint8_t test_buf[BUF_LENGTH_FRAME];
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < BUF_LENGTH_FRAME; j++){
            test_buf[j] = i;
        }
        shm_obj->deliver(test_buf);
    }
    printf("transfer test end \n");
    return;
}