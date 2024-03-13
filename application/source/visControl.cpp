#include "visControl.h"

#ifndef RAS_PI
//first try: using libgpiod.h in C
visGpioControl::visGpioControl(){
}

bool visGpioControl::init(){
    chip = gpiod_chip_open("/dev/gpiochip0");
    if(!chip){
        throw std::runtime_error("unable to get the gpio controller");
        return false;
    }
    return true;
}

bool visGpioControl::configPin(UWORD pin, bool write){
    static int n = 0;
    if(write){
        if(pin_write.find(pin) != pin_write.end())
            return true;
        if(pin_read.find(pin) != pin_read.end()){
            gpiod_line* prev_line = pin_read[pin];
            gpiod_line_release(prev_line);
            pin_read.erase(pin);
        }
        gpiod_line* target_pin = gpiod_chip_get_line(chip, pin);
        gpiod_line_request_output(target_pin, ("w" + std::to_string(n)).c_str(), 0);
        if(!target_pin){

            return false;
        }
        pin_write[pin] = target_pin;
    }
    else{
        if(pin_read.find(pin) != pin_read.end())
            return true;
        if(pin_write.find(pin) != pin_write.end()){
            gpiod_line* prev_line = pin_write[pin];
            gpiod_line_release(prev_line);
            pin_write.erase(pin);
        }
        gpiod_line* target_pin = gpiod_chip_get_line(chip, pin);
        gpiod_line_request_input(target_pin, ("r" + std::to_string(n)).c_str());
        if(!target_pin)
            return false;
        pin_read[pin] = target_pin;
    }
    return true;
}

UBYTE visGpioControl::readPin(UWORD pin){
    if(pin_read.find(pin) == pin_read.end())
        configPin(pin, false);
    gpiod_line* target_pin = pin_read[pin];
    int ret = gpiod_line_get_value(target_pin);
    if(ret == -1)
        printf("invalid read operation");
    return (UBYTE)ret;
}

bool visGpioControl::writePin(UWORD pin, UBYTE val){
    if(pin_write.find(pin) == pin_write.end())
        configPin(pin, true);
    gpiod_line* target_pin = pin_write[pin];
    int ret = gpiod_line_set_value(target_pin, val);
    if(ret == -1){
        printf("invalid write operation %d \n", pin);
        return false;
    }
    return true;
}

visGpioControl::~visGpioControl(){
    for(auto itr = this->pin_write.begin(); itr != this->pin_write.end(); itr++){
        gpiod_line_release(itr->second);
    }
    for(auto itr = this->pin_read.begin(); itr != this->pin_read.end(); itr++){
        gpiod_line_release(itr->second);
    }
    gpiod_chip_close(chip);
}


visI2CControl::visI2CControl(uint8_t a){
    dev_name = I2C_FILE;
    addr = a;
    fd = -1;
}

bool visI2CControl::init(){
    fd = open(dev_name.c_str(), O_RDWR);
    if(fd < 0){
        //in dev mode, in running mode change this part to runtime error
        std::cout<<"i2c device opening error"<<std::endl;
        return false;
    }
    //specify the i2c slave address using ioctl call

    if(ioctl(fd, I2C_SLAVE, addr) < 0){
        //TODO: change it in real demo
        std::cout<<"failed to acquire the bus"<<std::endl;
        return false;
    }
    return true;
}


bool visI2CControl:: write_i2c(uint8_t ad, uint8_t data){
    if(fd < 0){
        std::cout<<"invalid i2c object"<<std::endl;
        return false;
    }
    uint8_t buf[2] = {ad, data};
    std::cout<< "write to the i2c buf with return: "<<std::to_string(write(fd, buf, 2))<<std::endl;
    return false;
}



visI2CControl::~visI2CControl(){
    close(fd);
}

visScreenControl::visScreenControl(visGpioControl * gpio, visI2CControl* i2c){
    this->gpio = gpio;
    this->i2c_obj = i2c;
    image = new UBYTE[IMAGE_SIZE];
}

void visScreenControl::oledReset(){
    gpio->writePin(OLED_RST, 1); //oled rst, 1
    usleep(100 * M_SECOND);
    gpio->writePin(OLED_RST, 0); //oled rst, 1
    usleep(100 * M_SECOND);
    gpio->writePin(OLED_RST, 1); //oled rst, 1
    usleep(100 * M_SECOND);
}

void visScreenControl::oledWriteReg(uint8_t reg){
    i2c_obj->write_i2c(I2C_CMD, reg);
}


void visScreenControl::oledWriteData(uint8_t val){
    i2c_obj->write_i2c(I2C_CMD, val);
}



bool visScreenControl::init(){
    //gpio init
    gpio->writePin(OLED_CS, 1);
    gpio->writePin(OLED_RST, 1);
    gpio->writePin(OLED_DC, 1);

    //oled reset 
    printf("USE_IIC to start the screen\r\n");		
    gpio->writePin(OLED_DC, 0);
    gpio->writePin(OLED_CS, 0);


    oledReset();
    //init register
    oledWriteReg(0xAE); //turn off the oled panel
    oledWriteReg(0x00);
    oledWriteReg(0x10);//---set high column address
    
    oledWriteReg(0x20);
    oledWriteReg(0x00);
        
    oledWriteReg(0xFF);
    
    oledWriteReg(0xA6);
    
    oledWriteReg(0xA8); 
    oledWriteReg(0x3F);
    
    oledWriteReg(0xD3);
    oledWriteReg(0x00);
    
    oledWriteReg(0xD5);
    oledWriteReg(0x80);
    
    oledWriteReg(0xD9);
    oledWriteReg(0x22);

    oledWriteReg(0xDA);
    oledWriteReg(0x12);
    
    oledWriteReg(0xDB);
    oledWriteReg(0x40);
    //turn_on writeReg
    usleep(200 * M_SECOND);

    std::cout<<"turn_on screen"<<std::endl;
    oledWriteReg(0xAF);

    paintNewImage(270, BLACK);
    return true;
}


void visScreenControl::screenClear(){
    UWORD i, j;
    // Width = (OLED_1IN3_WIDTH % 8 == 0)? (OLED_1IN3_WIDTH / 8 ): (OLED_1IN3_WIDTH / 8 + 1);
    // Height = OLED_1IN3_HEIGHT; 
    for (i=0; i<8; i++) {
        /* set page address */
        oledWriteReg(0xB0 + i);
        /* set low column address */
        oledWriteReg(0x00);
        /* set high column address */
        oledWriteReg(0x10);
        for(j=0; j<128; j++) {
            /* write data */
            oledWriteData(0x00);  
        }
    }
}



void visScreenControl::paintNewImage(UWORD rotate, UWORD color)
{
    
    return;
}

void visScreenControl::oledDisplay(){
    UWORD page, column, temp;

    for (page=0; page<8; page++) {
        /* set page address */
        oledWriteReg(0xB0 + page);
        /* set low column address */
        oledWriteReg(0x00);
        /* set high column address */
        oledWriteReg(0x10);

        /* write data */
        for(column=0; column<128; column++) {
            temp = image[(7-page) + column*8];
            oledWriteData(temp);
        }       
    }
}

void visScreenControl:: paintClear(UWORD color){
    for (UWORD Y = 0; Y < IMAGE_HEIGHT; Y++) {
        for (UWORD X = 0; X < IMAGE_WIDTH; X++ ) {//8 pixel =  1 byte
            UDOUBLE Addr = X + Y*IMAGE_WIDTH;
            image[Addr] = color;
        }
    }
}

visScreenControl:: ~visScreenControl(){
    delete image;
    image = NULL;
}

#endif



/*
paint char
uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
*/


/*
void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if(Xpoint > Paint.Width || Ypoint > Paint.Height){
        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;

    switch(Paint.Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(Paint.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Paint.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Paint.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Paint.WidthMemory - X - 1;
        Y = Paint.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    if(Paint.Scale == 2){
        UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        if(Color == BLACK)
            Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(Paint.Scale == 4){
        UDOUBLE Addr = X / 4 + Y * Paint.WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = Paint.Image[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        Paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(Paint.Scale == 16) {
        UDOUBLE Addr = X / 2 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        Color = Color % 16;
        Rdata = Rdata & (~(0xf0 >> ((X % 2)*4)));
        Paint.Image[Addr] = Rdata | ((Color << 4) >> ((X % 2)*4));
    }else if(Paint.Scale == 65) {
        UDOUBLE Addr = X*2 + Y*Paint.WidthByte;
        Paint.Image[Addr] = 0xff & (Color>>8);
        Paint.Image[Addr+1] = 0xff & Color;
    }

}

*/







// void visScreenControl::paintDrawString(std::string text, int line){

// }



