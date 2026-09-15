//
// GPIO_Keypad : 3x3 keypad input and control LEDs (or Relays)
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// PA0,1,2,3,4,5 connected to 3x3 Keypad
// PC12,13,14,15 connected to LEDs (or Relays)

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"

void Init_GPIO(void)
{
	  GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
	  PC12=1; PC13=1; PC14=1; PC15=1;
}

void led(int a){
	PC12 =1; PC13 =1; PC14 =1; PC15 =1;
	
	switch(a){
		case 1 : PC12 =0; break;
		case 2 : PC13 =0; break;
	  case 3 : PC14 =0; break;
		case 4 : PC15 =0; break;
	}		
}

int main(void)
{
	uint32_t i =0;
	int old=0;
	int dir=0;
	int pau=1;
	int tmp=1;
	SYS_Init();
	OpenKeyPad();
	Init_GPIO();

 	while(1) 
  {
		i=ScanKey();
		if(i != 0 && old == 0){
			switch(i){
				case 1 : dir=1; pau=0; break;
				case 2 : 
					if(pau == 0){
						pau=1;
					}else{
						pau=0;                                                                                                                                                                                           
					}
					break;
				case 3 : dir=-1; pau=0; break;
			}	
		}
		
		old = i;
		
		if(pau==0){
			led(tmp);   
			CLK_SysTickDelay(100000);	 // Delay 
			
			if(dir == 1){
				tmp++;
				
				if(tmp > 4){
					tmp=1;
				}                       
			}else{
				tmp--;
				
				if(tmp < 1){
					tmp=4;
				}
			}
		}
	}
}
