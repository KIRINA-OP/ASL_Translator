cmake_minimum_required(VERSION 2.8)

PROJECT (Visapp)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/source SRC_LIST)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/unit_test UTEST_LIST)
set(GPIOD ${PROJECT_SOURCE_DIR}/include/gpiod/lib/libgpiod.a)

add_executable(visapp  ${PROJECT_SOURCE_DIR}/main.cpp ${SRC_LIST})
add_executable(unit_test ${SRC_LIST} ${UTEST_LIST})
target_compile_options(visapp PRIVATE -static)
target_compile_options(unit_test PRIVATE -static)

target_link_libraries(visapp ${GPIOD})
target_link_libraries(unit_test ${GPIOD})

