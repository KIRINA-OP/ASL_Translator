#include "visIpcMsg.h"
#include <cstdint>

//New Libs
#include <cstring>
#include <jpeglib.h>

visSocketApp:: visSocketApp(std::string app_path, std::string algo_path){
    app_sock_path = app_path;
    algo_sock_path = algo_path;
}

int visSocketApp::init(){
    if((app_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("app socket error");
        return -1;
    }
    memset(&appun, 0, sizeof(appun));  
    appun.sun_family = AF_UNIX;  
    strcpy(appun.sun_path, app_sock_path.c_str());  
    int len = offsetof(sockaddr_un, sun_path) + strlen(appun.sun_path);  
    unlink(appun.sun_path);  
    if (bind(app_sock, (struct sockaddr *)&appun, len) < 0) {  
        perror("bind error");  
        return -1;  
    }
    memset(&algoun, 0, sizeof(algoun));  
    algoun.sun_family = AF_UNIX;  
    strcpy(algoun.sun_path, algo_sock_path.c_str());  
    len = offsetof(sockaddr_un, sun_path) + strlen(algoun.sun_path);  
    if (connect(app_sock, (struct sockaddr *)&algoun, len) < 0){  
        perror("connect error");  
        exit(1);  
    } 
    return 0;  
}


int visSocketApp:: deliver(JSAMPLE * content, int len){
    write(app_sock, content, len);
    return 0;
}


int visSocketApp:: receive(JSAMPLE * buf, size_t len){
    int ret = read(app_sock, buf, len);
    if(ret < 0){
        perror("receive content error\n");
        return -1;
    }
    return 0;
}

visSocketApp:: ~visSocketApp(){
    close(app_sock);
}




visSocketAlgo:: visSocketAlgo(std::string app_path, std::string algo_path){
    app_sock_path = app_path;
    algo_sock_path = algo_path;
}

int visSocketAlgo:: init(){
    //since this class only needs to set connection with app_path, so it's okay for accept function inside here
    if((algo_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("algo socket error \n");
        return -1;
    }
    memset(&algoun, 0, sizeof(algoun));
    algoun.sun_family = AF_UNIX;
    strcpy(algoun.sun_path, algo_sock_path.c_str());
    int len = offsetof(struct sockaddr_un, sun_path) + strlen(algoun.sun_path);
    unlink(algo_sock_path.c_str());
    if(bind(algo_sock, (struct sockaddr*) & algoun, len) < 0){
        perror("algo socket bind error \n ");
        return -1;
    }
    printf("algo socket bound success \n");

    if(listen(algo_sock, 20) < 0){
        perror("listen error \n");
        return -1;
    }
    appun_len = sizeof(appun);
    if((app_sock = accept(algo_sock, (struct sockaddr *) & appun, &appun_len)) < 0){
        perror("accept error \n");
        return -1;
    }
    printf("accept app socket successfully \n");
    return 0;
}
int visSocketAlgo:: deliver(uint8_t * content, int len){
    write(app_sock, content, len);
    return 0;
}


int visSocketAlgo:: receive(uint8_t * buf, size_t len){
    int ret = read(app_sock, buf, len);
    if(ret < 0){
        perror("receive content error\n");
        return -1;
    }
    if(ret == 0){
        printf("EOF\n");
        return 1;
    }
    return 0;
}

visSocketAlgo:: ~visSocketAlgo(){
    close(app_sock);
    close(algo_sock);
}

