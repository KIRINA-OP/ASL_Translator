#include "visIpcMsg.h"
#include <cstdint>

//new
#include <fstream>
#include <iostream>
#include <ostream>

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


int visSocketApp:: deliver(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        perror("Error opening file for reading");
        return -1;
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Send the file size over the socket
    if (write(app_sock, &fileSize, sizeof(fileSize)) != sizeof(fileSize)) {
        perror("Error writing file size to socket");
        return -1;
    }

    // Allocate memory for the file content
    uint8_t *content = new uint8_t[fileSize];

    // Read the file content into memory
    file.read(reinterpret_cast<char*>(content), fileSize);

    // Send the file content over the socket
    ssize_t bytesSent = write(app_sock, content, fileSize);
    if (bytesSent != fileSize) {
        perror("Error writing file content to socket");
        delete[] content; // Clean up memory
        return -1;
    } else {
        std::cout << "Sent " << bytesSent << " bytes over the socket" << std::endl;
    }

    // Clean up
    delete[] content;
    return 0;
}


int visSocketApp::receive(const std::string& filename) {
    const int BUFFER_SIZE = 1024; // Define your buffer size as needed
    char buffer[BUFFER_SIZE];

    // Open file for writing the received data
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    // Receive the file size
    size_t fileSize;
    read(app_sock, &fileSize, sizeof(fileSize));

    // Receive data from the socket until the entire file is received
    size_t totalReceived = 0;
    ssize_t bytesRead;
    while (totalReceived < fileSize && (bytesRead = read(app_sock, buffer, BUFFER_SIZE)) > 0) {
        // Write the received data to the file
        file.write(buffer, bytesRead);
        totalReceived += bytesRead;
    }

    // Check for any errors or if the file size doesn't match what was received
    if (bytesRead < 0 || totalReceived != fileSize) {
        perror("Error receiving data from socket");
        return -1;
    }

    // Close the file
    file.close();

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

int visSocketAlgo:: deliver(const std::string& filename){
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        perror("Error opening file for reading");
        return -1;
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Send the file size over the socket
    if (write(app_sock, &fileSize, sizeof(fileSize)) != sizeof(fileSize)) {
        perror("Error writing file size to socket");
        return -1;
    }

    // Allocate memory for the file content
    uint8_t *content = new uint8_t[fileSize];

    // Read the file content into memory
    file.read(reinterpret_cast<char*>(content), fileSize);

    // Send the file content over the socket
    ssize_t bytesSent = write(app_sock, content, fileSize);
    if (bytesSent != fileSize) {
        perror("Error writing file content to socket");
        delete[] content; // Clean up memory
        return -1;
    } else {
        std::cout << "Sent " << bytesSent << " bytes over the socket" << std::endl;
    }

    // Clean up
    delete[] content;
    return 0;
}


int visSocketAlgo::receive(const std::string& filename) {
    const int BUFFER_SIZE = 1024; // Define your buffer size as needed
    char buffer[BUFFER_SIZE];

    // Open file for writing the received data
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    // Receive the file size
    size_t fileSize;
    read(app_sock, &fileSize, sizeof(fileSize));
    std::cout << fileSize << std::endl;

    // Receive data from the socket until the entire file is received
    size_t totalReceived = 0;
    ssize_t bytesRead;
    while (totalReceived < fileSize && (bytesRead = read(app_sock, buffer, BUFFER_SIZE)) > 0) {
        // Write the received data to the file
        file.write(buffer, bytesRead);
        totalReceived += bytesRead;
    }

    // Check for any errors or if the file size doesn't match what was received
    if (bytesRead < 0 || totalReceived != fileSize) {
        perror("Error receiving data from socket");
        return -1;
    }

    // Close the file
    file.close();

    return 0;
}

visSocketAlgo:: ~visSocketAlgo(){
    close(app_sock);
    close(algo_sock);
}

