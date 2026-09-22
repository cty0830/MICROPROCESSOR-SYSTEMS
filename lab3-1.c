
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"


void OpenSevenSegment(void);
void ShowSevenSegment(uint8_t no, uint8_t number);
void CloseSevenSegment(void);


extern uint8_t SEG_BUF[16];



void ForceShowDigit(uint8_t no, uint8_t number)
{
    uint8_t temp, i;

    temp = SEG_BUF[number];

    for(i = 0; i < 8; i++)
    {
        if(temp & 0x01)
        {
            switch(i)
            {
                case 0: PE0=1; break;
                case 1: PE1=1; break;
                case 2: PE2=1; break;
                case 3: PE3=1; break;
                case 4: PE4=1; break;
                case 5: PE5=1; break;
                case 6: PE6=1; break;
                case 7: PE7=1; break;
            }
        }
        else
        {
            switch(i)
            {
                case 0: PE0=0; break;
                case 1: PE1=0; break;
                case 2: PE2=0; break;
                case 3: PE3=0; break;
                case 4: PE4=0; break;
                case 5: PE5=0; break;
                case 6: PE6=0; break;
                case 7: PE7=0; break;
            }
        }

        temp >>= 1;
    }


    switch(no)
    {
        case 0: PC4=1; break;
        case 1: PC5=1; break;
        case 2: PC6=1; break;
        case 3: PC7=1; break;
    }
}



void Display_Stopwatch(uint8_t min, uint8_t sec)
{
    uint8_t digit3;
    uint8_t digit2;
    uint8_t digit1;
    uint8_t digit0;


    digit3 = min / 10;
    digit2 = min % 10;
    digit1 = sec / 10;
    digit0 = sec % 10;



    CloseSevenSegment();
    ForceShowDigit(3, digit3);
    CLK_SysTickDelay(2500);


    CloseSevenSegment();
    ForceShowDigit(2, digit2);
    CLK_SysTickDelay(2500);



    CloseSevenSegment();
    ForceShowDigit(1, digit1);
    CLK_SysTickDelay(2500);


    CloseSevenSegment();
    ForceShowDigit(0, digit0);
    CLK_SysTickDelay(2500);
}



int main(void)
{
    uint16_t key_pressed;
    uint16_t last_key = 0;

    int state = 0;


    uint8_t seconds = 50;
    uint8_t minutes = 0;

    uint32_t loop_tick = 0;


    SYS_Init();
    OpenSevenSegment();
    OpenKeyPad();


    while(1)
    {

        key_pressed = ScanKey();


        if(key_pressed != 0 && last_key == 0)
        {

            if(state == 0)
            {

                if(key_pressed == 1)
                {
                    state = 1;
                    loop_tick = 0;
                }
            }



            else if(state == 1)
            {

                if(key_pressed == 2)
                {
                    state = 2;
                }


                else if(key_pressed == 3)
                {
                    state = 0;

                    seconds = 0;
                    minutes = 0;
                    loop_tick = 0;
                }
            }

            else if(state == 2)
            {
                if(key_pressed == 1)
                {
                    state = 1;
                }
                else if(key_pressed == 3)
                {
                    state = 0;

                    seconds = 0;
                    minutes = 0;
                    loop_tick = 0;
                }
            }
        }

        last_key = key_pressed;

        if(state == 1)
        {
            loop_tick++;

            if(loop_tick >= 100)
            {
                loop_tick = 0;

                seconds++;

                if(seconds >= 60)
                {
                    seconds = 0;

                    minutes++;

                    if(minutes >= 100)
                    {
                        minutes = 0;
                    }
                }
            }
        }

        Display_Stopwatch(minutes, seconds);
    }
}
