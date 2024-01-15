cmake_minimum_required(VERSION 2.8)

#this script is only for shared memroy test 
PROJECT (visAlgo)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../application/include")

#APIs borrowed from application
set(SRC_LIST ${PROJECT_SOURCE_DIR}/../application/source/visIpcMsg.cpp)

add_executable(visalgo  ${PROJECT_SOURCE_DIR}/main.cpp ${SRC_LIST})