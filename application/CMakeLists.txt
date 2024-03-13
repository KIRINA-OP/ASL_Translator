cmake_minimum_required(VERSION 2.8)

PROJECT (Visapp)
set(CMAKE_SYSTEM_NAME Linux)
#on raspi, doesn't need this
# set(CMAKE_SYSTEM_PROCESSOR arm)
# set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
# set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
# INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include/lib")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/source SRC_LIST)
# AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/include/lib/*.c OLED_Config)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/unit_test UTEST_LIST)
add_compile_definitions(USE_DEV_LIB)

#in raspi doesn't need to include this
# set(GPIOD ${PROJECT_SOURCE_DIR}/include/gpiod/lib/libgpiod.a)

# SET(GPIO_FLAG    "-llgpio")
# add_definitions(${GPIO_FLAG})


add_executable(visapp  ${PROJECT_SOURCE_DIR}/main.cpp ${SRC_LIST} ${OLED_LIST})
# add_executable(unit_test ${SRC_LIST} ${UTEST_LIST} ${OLED_LIST})
# target_compile_options(visapp PRIVATE -static)
# target_compile_options(unit_test PRIVATE -static)
target_link_libraries(visapp lgpio)
target_link_libraries(visapp pthread)


#in raspi doesn't need to include this
# target_link_libraries(visapp ${GPIOD})
# target_link_libraries(unit_test ${GPIOD})

