//
// GPIO_EXTINT : External Interrupt Pins to interrupt MCU
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"



volatile uint32_t sysTick_ms = 0;


/*======================================================
  SysTick
======================================================*/

void SysTick_Handler(void)
{
    sysTick_ms++;
}


/*======================================================
  Delay
======================================================*/

void Delay_ms(uint32_t ms)
{
    uint32_t start;

    start = sysTick_ms;

    while((sysTick_ms - start) < ms)
    {
    }
}


/*======================================================
  ?????
======================================================*/

void Display_7seg(uint16_t value)
{
    uint8_t digit;


    // ??
    digit = value / 1000;

    CloseSevenSegment();
    ShowSevenSegment(3, digit);
    Delay_ms(2);


    // ??
    value = value - digit * 1000;

    digit = value / 100;

    CloseSevenSegment();
    ShowSevenSegment(2, digit);
    Delay_ms(2);


    // ??
    value = value - digit * 100;

    digit = value / 10;

    CloseSevenSegment();
    ShowSevenSegment(1, digit);
    Delay_ms(2);


    // ??
    value = value - digit * 10;

    digit = value;

    CloseSevenSegment();
    ShowSevenSegment(0, digit);
    Delay_ms(2);
}


/*======================================================
  ???
======================================================*/

void Display_Marquee(uint8_t seg)
{
    CloseSevenSegment();

    ShowMarqueeSegment(0, seg);
    Delay_ms(2);


    CloseSevenSegment();

    ShowMarqueeSegment(1, seg);
    Delay_ms(2);


    CloseSevenSegment();

    ShowMarqueeSegment(2, seg);
    Delay_ms(2);


    CloseSevenSegment();

    ShowMarqueeSegment(3, seg);
    Delay_ms(2);
}


/*======================================================
  ?????
======================================================*/

volatile int marquee_mode = 0;

volatile uint8_t marquee_seg = 0;

volatile uint32_t marquee_start = 0;


/*======================================================
  ???? EINT1
======================================================*/

void EINT1_IRQHandler(void)
{
    GPIO_CLR_INT_FLAG(PB, BIT15);

    marquee_mode = 1;

    marquee_seg = 0;

    marquee_start = sysTick_ms;
}


/*======================================================
  ???????
======================================================*/

void Init_EXTINT(void)
{
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);

    GPIO_EnableEINT1(
        PB,
        15,
        GPIO_INT_RISING
    );

    NVIC_EnableIRQ(EINT1_IRQn);


    GPIO_SET_DEBOUNCE_TIME(
        GPIO_DBCLKSRC_LIRC,
        GPIO_DBCLKSEL_64
    );

    GPIO_ENABLE_DEBOUNCE(PB, BIT15);
}


/*======================================================
  MAIN
======================================================*/

int main(void)
{
    uint16_t i;

    // ?????
    uint16_t old_key = 0;


    // ????,??:?
    uint32_t time = 0;


    // 1 = ??
    // 0 = ??
    int mode = 0;


    uint32_t minute;
    uint32_t sec;


    // ??????
    uint32_t timer_start = 0;


    /*==================================================
      ???
    ==================================================*/

    CloseSevenSegment();

    SYS_Init();

    OpenSevenSegment();

    Init_EXTINT();

    OpenKeyPad();


    // SysTick ? 1ms ????
    SysTick_Config(SystemCoreClock / 1000);


    /*==================================================
      ???
    ==================================================*/

    while(1)
    {
        i = ScanKey();


        /*================================================
          ?????
        =================================================*/

        if(marquee_mode == 1)
        {
            // ?????????
            if((sysTick_ms - marquee_start) >= 1000)
            {
                marquee_start += 1000;

                marquee_seg++;

                if(marquee_seg >= 6)
                {
                    marquee_seg = 0;
                }
            }


            // ?????
            Display_Marquee(marquee_seg);


            // ??????????
            if(i != 0)
            {
                marquee_mode = 0;

                marquee_seg = 0;

                CloseSevenSegment();


                // ??????
                while(ScanKey() != 0)
                {
                }
            }


            continue;
        }


        /*================================================
          KEY 1
          ????
        =================================================*/

        if(i == 1)
        {
            /*
             * ??????? KEY1 ??? timer_start
             *
             * ?????????,
             * ?? KEY1 ?????? timer_start?
             */
            if(old_key != 1)
            {
                mode = 1;

                timer_start = sysTick_ms;
            }
        }


        /*================================================
          KEY 2
          ????
        =================================================*/

        else if(i == 2)
        {
            mode = 0;
        }


        /*================================================
          KEY 3
          ??
        =================================================*/

        else if(i == 3)
        {
            mode = 0;

            time = 0;

            timer_start = sysTick_ms;
        }


        /*================================================
          KEY 4
          ?? 50
        =================================================*/

        else if(i == 4)
        {
            /*
             * ??????? KEY4 ???
             * ?????????
             */
            if(old_key != 4)
            {
                time += 50;
            }
        }


        /*================================================
          ??
        =================================================*/

        if(mode == 1)
        {
            /*
             * ??? 1000ms
             * time ?? 1 ?
             */
            if((sysTick_ms - timer_start) >= 1000)
            {
                time++;

                timer_start += 1000;
            }
        }


        /*================================================
          ???????
        =================================================*/

        old_key = i;


        /*================================================
          ???????
        =================================================*/

        minute = time / 60;

        sec = time % 60;


        /*================================================
          ?? MMSS
          ??:
          1?5? ? 0105
          10?20? ? 1020
        =================================================*/

        Display_7seg(minute * 100 + sec);
    }
}
