#include "visIpcMsg.h"
#include <cstdint>

/*struct visFrame{
    int length;
    int width;
    //other pointers given by opencv
};

class visSharedMemory: public visIpcMsg{
    //send Frame to AI process
    //this is only for read and write

    public:
    visSharedMemory();
    void init();
    void deliver();
    ~visSharedMemory();    
    
};
*/
const std::string PIPE_NAME = "vispipe";

visSharedMemory::visSharedMemory(int l){
    buf_length = l;
}


int visSharedMemory::init(){
    void* shm = NULL;
    shm_id = shmget((key_t)1234, buf_length, IPC_CREAT | IPC_EXCL); //open the shared memory
    if(shm_id == -1){
        printf("shmget err.\n");
        return 0;
    }
    shm = shmat(shm_id, (void*) 0, 0);
    if(shm == (void*)-1)
	{
		printf("shmat err.\n");
		return 0;
	}
    shm_msg = (struct visShmMsg*) shm;
    shm_msg->chBuffer = new uint8_t[buf_length];
    shm_msg->shmSig = 0;
    return 1;
}

int visSharedMemory::deliver(uint8_t * content){
    if(getShSig() == 1)
        return 0; //indicating the shared memory is not yet ready
    std::memcpy(shm_msg->chBuffer, content, buf_length);
    shm_msg->shmSig = 1;
    return 1;
}

uint8_t * visSharedMemory::receive(){
    if(getShSig() == 0)
        return NULL;//indicating right now there is no valid data
    shm_msg->shmSig = 0;
    uint8_t * ret = new uint8_t[buf_length];
    memcpy(ret, shm_msg->chBuffer, buf_length);
    return ret;
}



visSharedMemory:: ~visSharedMemory(){
    shmdt((void *)shm_msg);
    delete (uint8_t*)shm_msg->chBuffer;
    std::cout<<"terminate shared memory gate"<<std::endl;
}    
