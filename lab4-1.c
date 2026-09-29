//
// GPIO_EXTINT : External Interrupt Pins to interrupt MCU
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
// INT0 /PB14 : NUC140 LQFP100 pin4
// INT1 /PB15 : NUC140 LQFP100 pin91

// #define SEG_N16  0x00 #define SEG_N17  0x63 
// uint8_t SEG_BUF[18]={SEG_N0, SEG_N1, SEG_N2, SEG_N3, SEG_N4, SEG_N5, SEG_N6, SEG_N7, SEG_N8, SEG_N9, SEG_N10, SEG_N11, SEG_N12, SEG_N13, SEG_N14, SEG_N15, SEG_N16, SEG_N17}; 
// 增加定義兩個角位 8. 和 度


#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"

void Display_7seg(uint16_t value)
{
    uint8_t digit;
    digit = value / 1000;
    CloseSevenSegment();
    ShowSevenSegment(3,digit);
    CLK_SysTickDelay(5000);
            
    value = value - digit * 1000;
    digit = value / 100;
    CloseSevenSegment();
    ShowSevenSegment(2,digit);
    CLK_SysTickDelay(5000);

    value = value - digit * 100;
    digit = value / 10;
    CloseSevenSegment();
    ShowSevenSegment(1,digit);
    CLK_SysTickDelay(5000);

    value = value - digit * 10;
    digit = value;
    CloseSevenSegment();
    ShowSevenSegment(0,digit);
    CLK_SysTickDelay(5000);
}

int display_timer = 0;
void EINT1_IRQHandler(void)
{
    GPIO_CLR_INT_FLAG(PB, BIT15);  
    display_timer = 150; 
}

void Init_EXTINT(void)
{
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);
    GPIO_EnableEINT1(PB, 15, GPIO_INT_RISING); 
    NVIC_EnableIRQ(EINT1_IRQn);

    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);
}

int32_t main()
{   
    SYS_Init();
    Init_EXTINT();
    CloseSevenSegment();

    while(1)
    {
        if (display_timer > 0) 
        {
            CloseSevenSegment(); 
            ShowSevenSegment(3, 16); 
            CLK_SysTickDelay(5000); 

            CloseSevenSegment(); 
            ShowSevenSegment(2, 5);  
            CLK_SysTickDelay(5000); 

            CloseSevenSegment(); 
            ShowSevenSegment(1, 17); 
            CLK_SysTickDelay(5000); 

            CloseSevenSegment(); 
            ShowSevenSegment(0, 12); 
            CLK_SysTickDelay(5000); 

            display_timer--;
            if (display_timer == 0) {
                CloseSevenSegment();
            }
        }
    }
}
