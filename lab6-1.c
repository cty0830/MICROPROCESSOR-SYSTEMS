//
// LCD: display Text on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

void clear_result_lines(void) {
    print_Line(1, "                ");
    print_Line(2, "                ");
    print_Line(3, "                ");
}

int main(void)
{
    uint32_t i = 0;
    int old = 0;
    int bit[2] = {0};      
    int count = 0;         
    char lcd_buffer[16];   

    SYS_Init();
    init_LCD();
    clear_LCD();
    OpenKeyPad();

    print_Line(0, "D1348859");
    while(1) 
    {
        i = ScanKey();
        if(i != 0 && old == 0){
            switch(i){
                case 1: 
                    if (count < 2) {
                        bit[count] = 0;
                        count++;
                    }
                    break;
                case 2: 
                    if (count < 2) {
                        bit[count] = 1;
                        count++;
                    }
                    break;
                case 3: 
                    count = 0;
                    clear_result_lines();
                    break;
            }    
        }
        old = i;

        if(count == 2){
            int bit1 = bit[0];
            int bit2 = bit[1];
           
            sprintf(lcd_buffer, "%d AND %d = %d", bit1, bit2, bit1 & bit2);
            print_Line(1, lcd_buffer);
            
            sprintf(lcd_buffer, "%d OR  %d = %d", bit1, bit2, bit1 | bit2);
            print_Line(2, lcd_buffer);

            sprintf(lcd_buffer, "%d XOR %d = %d", bit1, bit2, bit1 ^ bit2);
            print_Line(3, lcd_buffer);
        }
    }
}
