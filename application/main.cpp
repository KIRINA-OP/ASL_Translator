#include "main.h"
#include "visControl.h"
#include <string>
#include "lib/Config/DEV_Config.h"
#include "lib/GUI/GUI_Paint.h"
#include "lib/GUI/GUI_BMPfile.h"
#include "lib/Config/Debug.h"
#include "lib/OLED/OLED_1in51.h"

// #include "visThreads.h"

// visScreenControl* screenInit(visGpioControl* gpio, visI2CControl* i2c){
//     //init the gpio struct
//     visScreenControl* ret = new visScreenControl(gpio, i2c);
//     ret->init();
//     //init the screen control
//     ret->screenClear();
//     //init the camera
//     ret->paintClear(WHITE);
//     ret->oledDisplay();
//     usleep(1000*M_SECOND);
//     return ret;
// }



int main(){
	printf("1.51inch OLED test demo\n");
	if(DEV_ModuleInit() != 0) {
		return -1;
	}
	  
	printf("OLED Init...\r\n");
	OLED_1in51_Init();
	DEV_Delay_ms(500);
    OLED_1in51_Clear();	
	// 0.Create a new image cache
	UBYTE *BlackImage;
	UWORD Imagesize = ((OLED_1in51_WIDTH%8==0)? (OLED_1in51_WIDTH/8): (OLED_1in51_WIDTH/8+1)) * OLED_1in51_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\r\n");
			return -1;
	}
	printf("Paint_NewImage\r\n");
	Paint_NewImage(BlackImage, OLED_1in51_WIDTH, OLED_1in51_HEIGHT, 270, BLACK);	
	// Paint_SetScale(16);
	printf("Drawing\r\n");
	//1.Select Image
	Paint_SelectImage(BlackImage);
	DEV_Delay_ms(500);
	Paint_Clear(BLACK);
    
    // 2.Drawing on the image   
    printf("Drawing:page 1\r\n");
    Paint_DrawPoint(20, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(30, 10, WHITE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(40, 10, WHITE, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(10, 10, 10, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(20, 20, 20, 30, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(30, 30, 30, 40, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(40, 40, 40, 50, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawCircle(60, 30, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(100, 40, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);      
    Paint_DrawRectangle(50, 30, 60, 40, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(90, 30, 110, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);   
    // 3.Show image on page1
    OLED_1in51_Display(BlackImage);
    DEV_Delay_ms(2000);      
    Paint_Clear(BLACK);
    
    // Drawing on the image
    printf("Drawing:page 2\r\n");     
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, WHITE, WHITE);
    Paint_DrawString_EN(10, 17, "hello world", &Font8, WHITE, WHITE);
    Paint_DrawNum(10, 30, 123.456789, &Font8, 4, WHITE, WHITE);
    Paint_DrawNum(10, 43, 987654, &Font12, 5, WHITE, WHITE);
    // Show image on page2
    OLED_1in51_Display(BlackImage);
    DEV_Delay_ms(2000);  
    Paint_Clear(BLACK);  

    // Drawing on the image
    printf("Drawing:page 4\r\n");
    GUI_ReadBmp("./pic/1in51.bmp", 0, 0);
	OLED_1in51_Display(BlackImage);
    DEV_Delay_ms(2000);
    Paint_Clear(BLACK); 

    OLED_1in51_Clear();
	
    return 0;
}

