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

int main(void)
{
	uint16_t i;
	uint16_t num = 0;
	int count = 0;
	int old = 0;

	SYS_Init();
	OpenSevenSegment();
	OpenKeyPad();

	while(1)
	{
	i = ScanKey();

	if(i != 0 && old == 0)
	{
	
		if(i >= 1 && i <= 6)
		{
	
			if(count < 4)
			{
				num = num * 10 + i;
				count++;
			}
			
		}
	
		else if(i == 7)
		{
	
			if(count > 0)
			{
			num = num / 10;
			count--;
			}
		}
	}

	old = i;
	Display_7seg(num);
	}
}
