//
// LCD_Bmp : display Logo Bitmap on LCD
//
// Bitmap Creation:
// Step 1. use Paint to draw 128x64 bitmap 
// Step 2. use bmp2asm to convert bitmap into hex
// Step 3. cut hex code paste into char array Bitmap[128*8]
// Step 4. build & download code to learning board, then bitmap will be displayed on LCD
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

unsigned char dvd[32*4] = { 
	0x00,0x00,0x00,0x78,0x78,0x78,0x78,0x78,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x80,0x00,0x00,0x00,0x80,0xE0,0xF8,0xF8,0x78,0x78,0x78,0x78,0x78,0x78,0xF0,0xE0,0x80,0x00,
	0x00,0x80,0xFF,0xFF,0xFF,0x80,0xC0,0xC0,0xF0,0xFF,0x7F,0x0F,0x07,0xFF,0xFF,0xE0,0xFC,0xFC,0x3F,0x0F,0x03,0xFC,0xFF,0xFF,0x81,0x80,0xC0,0xE0,0xFF,0x7F,0x1F,0x00,
	0x00,0x03,0x83,0x83,0x83,0xC3,0xC3,0xC3,0xC1,0xC0,0xC0,0xC0,0xC0,0xE0,0xFF,0xE7,0xC1,0xC1,0xC0,0xC0,0xC0,0xC3,0xC3,0xC3,0xC3,0x83,0x83,0x01,0x01,0x00,0x00,0x00,
	0x02,0x07,0x07,0x0F,0x0F,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F,0x19,0x19,0x18,0x18,0x19,0x19,0x19,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x0F,0x0F,0x07,0x07,0x00,0x00,0x00
};

void RGB_Blue(void){
    PA12 = 0;
    PA13 = 1;
    PA14 = 1;
}

void RGB_Red(void){
    PA12 = 1;
    PA13 = 1;
    PA14 = 0;
}

void RGB_Green(void){
    PA12 = 1;
    PA13 = 0;
    PA14 = 1;
}
	
void RGB_Bg(void){
    PA12 = 0;
    PA13 = 0;
    PA14 = 1;
}

void RGB_yellow(void){
    PA12 = 1;
    PA13 = 0;
    PA14 = 0;
}

void RGB_Rb(void){
    PA12 = 0;
    PA13 = 1;
    PA14 = 0;
}

void RGB_Change(int color){
    if(color == 0){
        RGB_Blue();
    }else if(color == 1){
        RGB_Bg();
    }else if(color == 2){
        RGB_Green();
    }else if(color == 3){
        RGB_yellow();
    }else if(color == 4){
        RGB_Red();
    }else if(color == 5){
        RGB_Rb();
    }
}

int main(void)
{
		int hit;
    int i = 0;
    int old = 0;
	  int x = 48;
		int y = 16;
    int dx = 0;
    int dy = 0;
    int color = 0;

    SYS_Init();
    init_LCD();
    clear_LCD();
	
    while(1){
			hit = 0;
      i = ScanKey();
			
      if(i != 0 && old == 0) {
				if(i == 1){
					dx = -1;
          dy = -1;
        }else if(i == 3){
          dx = 1;
          dy = -1;
        }else if(i == 5){
          x = 48;
          y = 16;
          dx = 0;
          dy = 0;
					color = 0;
        }else if(i == 7) {
					dx = -1;
					dy = 1;
        }else if(i == 9){
           dx = 1;
           dy = 1;
        }
      }

      old = i;

      x += dx;
      y += dy;

			if(x <= 0){
				x = 0;
				dx = -dx;
				hit = 1;
			}else if(x >= 96){
				x = 96;
				dx = -dx;
				hit = 1;
			}
	
			if(y <= 0){
				y = 0;
				dy = -dy;
				hit = 1;
			}else if(y >= 32){
				y = 32;
				dy = -dy;
				hit = 1;
			}

			if(hit == 1){
				color++;
				color = color % 6;		
			}
				
			RGB_Change(color);
      clear_LCD();
      draw_Bmp32x32(x, y, 1, 0, dvd);
      CLK_SysTickDelay(100000);
    }
}
