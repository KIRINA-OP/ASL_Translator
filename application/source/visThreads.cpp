#include "visThreads.h"
#include "visIpcMsg.h"

//New libs
#include <cstdio>
#include <jpeglib.h>

//Define image size
#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480

//this is the test function
void getFrameBuf(JSAMPLE * buf, const int len){
    
    for (int y = 0; y < IMAGE_HEIGHT; ++y) {
        for (int x = 0; x < IMAGE_WIDTH; ++x) {
            int index = (y * IMAGE_WIDTH + x) * 3;  // Assuming RGB with 3 components per pixel

            // Example: Set RGB values (replace this with your actual pixel data)
            buf[index]     = static_cast<JSAMPLE>(x % 256);  // Red
            buf[index + 1] = static_cast<JSAMPLE>(y % 256);  // Green
            buf[index + 2] = static_cast<JSAMPLE>((x + y) % 256);  // Blue
        }
    }

    // JPEG compression settings
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    // Set compression parameters
    cinfo.image_width = IMAGE_WIDTH;
    cinfo.image_height = IMAGE_HEIGHT;
    cinfo.input_components = 3;  // RGB components per pixel
    cinfo.in_color_space = JCS_RGB;

    // Set destination file (replace with your own file I/O if needed)
    FILE *outfile = fopen("output.jpg", "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output jpeg file\n");
        exit(EXIT_FAILURE);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    // Set JPEG compression options
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);  // Adjust quality as needed

    // Start compression
    jpeg_start_compress(&cinfo, TRUE);

    // Write scanlines
    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPLE *row_pointer = &buf[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    // Finish compression
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(outfile);

    return;
}

void print_buf(unsigned char * buf, int len){
    for(int i = 0; i < len; i++){
        printf("%x ", buf[i]);
    }
    printf("end \n");
}

void frameTransfer(){
    visSocketApp app_socket = visSocketApp(APP_SOCKET_PATH, ALGO_SOCKET_PATH);
    app_socket.init();
    JSAMPLE * deliver_buf = new uint8_t[BUF_LENGTH_FRAME];
    //start two threads one for sending and one for receiving
    getFrameBuf(deliver_buf, BUF_LENGTH_FRAME);
    app_socket.deliver(deliver_buf, BUF_LENGTH_FRAME);
    sleep(1);
    JSAMPLE * receive_buf = new uint8_t[MAXLINE];
    app_socket.receive(receive_buf, MAXLINE);
    printf("app_received: ");
    print_buf(receive_buf, MAXLINE);
    delete[] deliver_buf;
    deliver_buf = NULL;
    delete[] receive_buf;
    receive_buf = NULL;
    return;
}