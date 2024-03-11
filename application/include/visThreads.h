#ifndef VISTHREAD_H
#define VISTHREAD_H

//TODO: implement in threadpool, in controller ticket
#include "main.h"
#include "visIpcMsg.h"
#include <jpeglib.h>

void print_buf(uint8_t * buf, int len);
void getFrameBuf(JSAMPLE * buf, const int len);
void frameTransfer();

#endif