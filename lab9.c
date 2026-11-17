#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include "NUC100Series.h"

#include "MCU_init.h"

#include "SYS_init.h"

#include "LCD.h"

#include "Scankey.h"


// ???? LCD ???? (?? Keil ?????)

extern void draw_Circle(int16_t x, int16_t y, int16_t r, uint16_t fg, uint16_t bg);

extern void draw_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t fg, uint16_t bg);

extern void fill_Rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t fg, uint16_t bg);


#define PI 3.14159265

#define X0 64

#define Y0 32

#define R_CENTER 5

#define R_BLADE  28


// RGB LED ?? (PA12=B, PA13=G, PA14=R, 0? 1?)

void LED_Off(void)   { PA12 = 1; PA13 = 1; PA14 = 1; }

void LED_White(void) { PA12 = 0; PA13 = 0; PA14 = 0; }


// ??? ADC (?????????? VR1 / PA7 / Channel 7)

void Init_ADC(void)

{

    GPIO_SetMode(PA, BIT7, GPIO_MODE_INPUT);

    GPIO_DISABLE_DIGITAL_PATH(PA, BIT7);

    SYS->GPA_MFP |= SYS_GPA_MFP_PA7_ADC7;

    

    CLK_EnableModuleClock(ADC_MODULE);

    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HXT, CLK_CLKDIV_ADC(1));

    

    ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_SINGLE, BIT7);

    ADC_POWER_ON(ADC);

}


// ???? VR1 ? ADC ?? (0 ~ 4095)

uint32_t Read_VR1_ADC(void)

{

    uint32_t val;

    ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

    ADC_START_CONV(ADC);

    while (!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

    val = ADC_GET_CONVERSION_DATA(ADC, 7);

    return val;

}


// ???? (??? + ???? 120 ???????)

void Draw_Fan(int16_t base_angle, uint16_t fg, uint16_t bg)

{

    uint8_t i;

    int16_t deg1, deg2;

    double rad1, rad2;

    int16_t x1, y1, x2, y2;


    // ?????? 5 ??

    draw_Circle(X0, Y0, R_CENTER, fg, bg);


    // ????? (0?, 120?, 240?)

    for (i = 0; i < 3; i++) {

        deg1 = base_angle + (i * 120) - 15; // ???????

        deg2 = base_angle + (i * 120) + 15; // ???????


        rad1 = deg1 * PI / 180.0;

        rad2 = deg2 * PI / 180.0;


        x1 = X0 + (int16_t)(R_BLADE * cos(rad1));

        y1 = Y0 + (int16_t)(R_BLADE * sin(rad1));

        x2 = X0 + (int16_t)(R_BLADE * cos(rad2));

        y2 = Y0 + (int16_t)(R_BLADE * sin(rad2));


        draw_Triangle(X0, Y0, x1, y1, x2, y2, fg, bg);

    }

}


int main(void)

{

    uint16_t key = 0, last_key = 0;

    uint8_t power_on = 0;          // 0:??, 1:??

    

    int16_t angle = 0;             // ????????

    int16_t current_speed = 0;     // ?????? (??????)

    int16_t target_speed = 0;      // ????

    uint8_t accel_tick = 0;        // ????????


    uint32_t adc_val = 0;          // ?? ADC (0~4095)

    uint32_t mapped_val = 0;       // ????? (0~5000)

    char text_buf[16];


    SYS_Init();

    OpenKeyPad();

    init_LCD();

    clear_LCD();

    Init_ADC();


    // ?? RGB LED ?????????? (????)

    GPIO_SetMode(PA, BIT12 | BIT13 | BIT14, GPIO_MODE_OUTPUT);

    LED_Off();


    // ????:??????

    Draw_Fan(angle, FG_COLOR, BG_COLOR);


    while (1)

    {

        // ----------------------------------------------------

        // 1. ???? (??1: ?? / ??2: ??)

        // ----------------------------------------------------

        key = ScanKey();

        if (last_key != 0 && key == 0)

        {

            if (last_key == 1)      // ?? 1:??

            {

                power_on = 0;

                LED_Off();          // RGB LED ??

            }

            else if (last_key == 2) // ?? 2:??

            {

                power_on = 1;

                LED_White();        // RGB LED ???

            }

        }

        last_key = key;


        // ----------------------------------------------------

        // 2. ?? ADC ??? 0 ~ 5000 ? LCD ???

        // ----------------------------------------------------

        adc_val = Read_VR1_ADC();

        mapped_val = (adc_val * 5000UL) / 4095UL;


        // ???? 4 ??????????? (Line 3, X ??? 96)

        sprintf(text_buf, "%4d", (int)mapped_val);

        printS(96, 48, text_buf); // ??? LCD.h ??? print_Line(3, ...) ???????


        // ----------------------------------------------------

        // 3. ?????? (??????? ADC ??)

        // ----------------------------------------------------

        if (power_on == 0)

        {

            target_speed = 0; // ???????? 0 (????)

        }

        else

        {

            if (adc_val <= 1023)      target_speed = 3;   // ?

            else if (adc_val <= 2047) target_speed = 7;   // ?

            else if (adc_val <= 3071) target_speed = 12;  // ?

            else                      target_speed = 18;  // ????

        }


        // ----------------------------------------------------

        // 4. ?????? / ???? (????)

        // ----------------------------------------------------

        accel_tick++;

        if (accel_tick >= 3) // ? 3 ????? 1 ????,??????

        {

            accel_tick = 0;

            if (current_speed < target_speed) {

                current_speed++;

            } else if (current_speed > target_speed) {

                current_speed--;

            }

        }


        // ----------------------------------------------------

        // 5. ???????? (??????,?????)

        // ----------------------------------------------------

        if (current_speed > 0)

        {

            // ???????????? (???? clear_LCD ????)

            Draw_Fan(angle, BG_COLOR, BG_COLOR);


            // ????

            angle = (angle + current_speed) % 360;


            // ????????????

            Draw_Fan(angle, FG_COLOR, BG_COLOR);

        }


        CLK_SysTickDelay(25000); // ??????? (? 25ms)

    }

}
