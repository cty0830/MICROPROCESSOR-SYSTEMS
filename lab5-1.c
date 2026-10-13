//
// GPIO_7seg_keypad : 3x3 keypad inpt and display on 7-segment LEDs
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"

// display an integer on four 7-segment LEDs
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

void play_do(){
    uint32_t j;
    uint32_t count = 1000000 / (2 * 956); 
    
    for(j = 0; j < count; j++) {
        PB11 = 0;
        CloseSevenSegment(); 
        ShowSevenSegment(0, 0); 
        CLK_SysTickDelay(956); 

        PB11 = 1;
        CloseSevenSegment(); 
        ShowSevenSegment(1, 13);  
        CLK_SysTickDelay(956);
    }
    CloseSevenSegment(); 
}

void play_re(){
    uint32_t j;
    uint32_t count = 1000000 / (2 * 852); 
    
    for(j = 0; j < count; j++) {
        PB11 = 0;
        CloseSevenSegment(); 
        ShowSevenSegment(0, 14); 
        CLK_SysTickDelay(852); 

        PB11 = 1;
        CloseSevenSegment(); 
        ShowSevenSegment(1, 1);  
        CLK_SysTickDelay(852);
    }
    CloseSevenSegment(); 
}

void play_so(){
    uint32_t j;
    uint32_t count = 1000000 / (2 * 638); 
    
    for(j = 0; j < count; j++) {
        PB11 = 0;
        CloseSevenSegment(); 
        ShowSevenSegment(0, 0); 
        CLK_SysTickDelay(638); 

        PB11 = 1;
        CloseSevenSegment(); 
        ShowSevenSegment(1, 5);  
        CLK_SysTickDelay(638);
    }
    CloseSevenSegment(); 
}

void play_minso(){
    uint32_t j;
    uint32_t count = 1000000 / (2 * 1276); 
    
    for(j = 0; j < count; j++) {
        PB11 = 0;
        CloseSevenSegment(); 
        ShowSevenSegment(0, 0); 
        CLK_SysTickDelay(1276); 

        PB11 = 1;
        CloseSevenSegment(); 
        ShowSevenSegment(1, 5);  
        CLK_SysTickDelay(1276);
    }
    CloseSevenSegment(); 
}

int main(void)
{
		uint16_t i;
		uint16_t old = 0;
	
    SYS_Init();
    OpenSevenSegment();
	  OpenKeyPad();

 	  while(1) {
			i=ScanKey();
			
			if(i == 0 && old != 0) {
				if (old == 1) {
          play_do();   
					CLK_SysTickDelay(1000000);
					play_so();
				} else if (old == 2){
					play_re();   
					CLK_SysTickDelay(1000000);
					play_minso();
			}
	  }
			
		old = i;
	}
}
