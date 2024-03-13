#ifndef UNITTEST_H
#define UNITTEST_H
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

void setTimer(int seconds, int mseconds);
void test_oled();

#endif