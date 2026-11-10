//
// LCD_keypad : Radar
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

#define PI 3.14159265
#define X0 64
#define Y0 32
#define R 30

int old_x, old_y;
int new_x, new_y;
int radar_x = 74, radar_y = 22;
int target_theta;
int theta = 0;
int mode = 0;
int hit = 0;
int show_time = 0;

void Init_Buzzer(void){
    GPIO_SetMode(PB, BIT11, GPIO_PMD_OUTPUT);
    PB11 = 1;
}

void Buzz(void){
    PB11 = 0;
    CLK_SysTickDelay(100000);
    PB11 = 1;
}

void start(void){
    draw_Circle(64, 32, 30, 1, 0);
    draw_Line(34, 32, 94, 32, 1, 0);
    draw_Line(64, 2, 64, 62, 1, 0);
    draw_Triangle(10, 0, 0, 0, 0, 10, 1, 0);
    draw_Triangle(0, 53, 0, 63, 10, 63, 1, 0);
    draw_Triangle(117, 0, 127, 0, 127, 10, 1, 0);
    draw_Triangle(117, 63, 127, 63, 127, 53, 1, 0);
}

void random_point(void){
   int dx, dy;

   do {
		dx = rand() % 55 - 27;
    dy = rand() % 55 - 27;
   } while (dx * dx + dy * dy > 27 * 27);

   radar_x = X0 + dx;
   radar_y = Y0 + dy;

   target_theta = (int)(atan2((double)dx, (double)-dy) * 180.0 / PI + 0.5);

	 if (target_theta < 0) {
		target_theta += 360;
	 }
}

void show_point(void){
    draw_Line(radar_x, radar_y, radar_x + 1, radar_y, 1, 0);
    draw_Line(radar_x, radar_y + 1, radar_x + 1, radar_y + 1, 1, 0);
}

void erase_point(void){
    draw_Line(radar_x, radar_y, radar_x + 1, radar_y, 0, 0);
    draw_Line(radar_x, radar_y + 1, radar_x + 1, radar_y + 1, 0, 0);
}

void check(int theta){
    new_x = X0 + R * sin(theta * PI / 180.0);
		new_y = Y0 - R * cos(theta * PI / 180.0);

    draw_Line(old_x, old_y, X0, Y0, 0, 0);
    start();
    draw_Line(new_x, new_y, X0, Y0, 1, 0);

    old_x = new_x;
    old_y = new_y;

    if (hit == 1) {
        show_point();
    }
}

int main(void)
{
    uint8_t i = 0;
    uint8_t old = 0;
    int d;
		int count = 0;

    SYS_Init();
    init_LCD();
    clear_LCD();
    OpenKeyPad();
    Init_Buzzer();
	
    old_x = X0;
		old_y = Y0 - R;
	
		target_theta = (int)(atan2((double)(radar_x - X0),(double)(Y0 - radar_y)) * 180.0 / PI + 0.5);

		if (target_theta < 0) {
			target_theta += 360;
		}

    start();
    while (1){
			i = ScanKey();
			count++;
			
      if (i != 0 && old == 0){
				if (i == 1){
					mode = 1;
        }else if (i == 2){
          mode = 0;
					theta = 0;
					hit = 0;
          show_time = 0;
					radar_x = 74;
					radar_y = 22;
					target_theta = (int)(atan2((double)(radar_x - X0),(double)(Y0 - radar_y)) * 180.0 / PI + 0.5);
					if (target_theta < 0) {
							target_theta += 360;
					}
          old_x = X0;
					old_y = Y0 - R;
          clear_LCD();
          start();
        }else if (i == 3){
          srand(count);
					random_point();

          hit = 0;
          show_time = 0;

          clear_LCD();
          start();
        }
     }
     
		 old = i;

     if (mode == 1){
			check(theta);
			d = abs(theta - target_theta);

			if (d > 180) {
				d = 360 - d;
			}
			
		  if (d <= 1 && hit == 0){
				hit = 1;
				show_time = 0;

				Buzz();
				show_point();
			}
			
			if (hit == 1){
				show_time++;

				if (show_time >= 40){
					erase_point();
					hit = 0;
					show_time = 0;
				}
			}

			theta++;

			if (theta >= 360) {
				theta = 0;
			}
		}
       CLK_SysTickDelay(50000);
    }
}
