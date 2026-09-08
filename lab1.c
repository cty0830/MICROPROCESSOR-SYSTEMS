//
// GPIO_LED : GPIO output to control an on-board red LED
// 
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// low-active output control by GPC12

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"

int main(void)
{
    SYS_Init(); 
    GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	
	  // 1
    while(1) {
	  PC12 =0; // turn on LED
	  PC15 =0; // turn off LED		
		}
  
		//2
		while(1) {
	  PC12 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC12 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay 	
		PC13 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC13 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		PC14 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC14 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		PC15 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC15 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		}
		
		//3
		while(1) {
	  PC15 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC15 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay 	
		PC14 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC14 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		PC13 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC13 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		PC12 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PC12 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay
		}
}
