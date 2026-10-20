// // LCD: display Text on LCD //
// // EVB : Nu-LB-NUC140 //
// // MCU : NUC140VE3CN (LQPF-100)
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

void clear_result_lines(void) {
    print_Line(2, "                "); 
    print_Line(3, "                "); 
}

void clear_password_stars(void) {
    print_Line(1, "Password:       "); 
}

void Display_7seg(uint16_t value) {
    uint8_t digit;
    
    digit = value / 1000;
    CloseSevenSegment();
    ShowSevenSegment(3, digit);
    CLK_SysTickDelay(5000);
    
    value = value - digit * 1000;
    digit = value / 100;
    CloseSevenSegment();
    ShowSevenSegment(2, digit);
    CLK_SysTickDelay(5000);
    
    value = value - digit * 100;
    digit = value / 10;
    CloseSevenSegment();
    ShowSevenSegment(1, digit);
    CLK_SysTickDelay(5000);
    
    value = value - digit * 10;
    digit = value;
    CloseSevenSegment();
    ShowSevenSegment(0, digit);
    CLK_SysTickDelay(5000);
}

int main(void) {
    uint32_t i = 0;
    int old = 0;
    
    int mod = 0;       
    int count = 0;     
    int pass = 0;     
    int input = 0;     
    int x = 72;       
	
    SYS_Init();
    init_LCD();
    clear_LCD();
    OpenKeyPad();
    CloseSevenSegment();
    
    print_Line(0, "Unlock");
    print_Line(1, "Password: ");
    
    while(1) {
        i = ScanKey();
        
        if (i != 0 && old == 0) {  
            if (mod == 0) {
                if (i >= 1 && i <= 6) {
                    if (count < 4) {
                        clear_result_lines(); 
                        pass = pass * 10 + i;
                        count++;
                        printC(x, 16, '*');  
                        x = x + 8;
                    }
                } else if (i == 7) {
                    if (count == 4) {
                        mod = 1; 
                        print_Line(0, "Lock");
                        clear_password_stars();
                        clear_result_lines();
                        x = 72;
                        input = 0; 
                    } else {
                        print_Line(3, "Bad password!");
                        clear_password_stars();
                        pass = 0;
                        count = 0;
                        x = 72;
                    }
                }
            } else if (mod == 1) {
                if (i >= 1 && i <= 6) {
                    clear_result_lines();
                    input = input * 10 + i;
                    printC(x, 16, '*');
                    x = x + 8;
                } else if (i == 7) {
                    if (input == pass) {
                        mod = 0;
                        print_Line(0, "Unlock");
                        clear_password_stars();
                        clear_result_lines();
                        pass = 0;
                        count = 0;
                        x = 72;
                    } else {
                        print_Line(3, "Access denied!");
                        clear_password_stars();
                        input = 0;
                        x = 72;
                    }
                }
            }
        }
        
        old = i; 
      
        if (mod == 0) {
            Display_7seg(pass);
        } else {
            if (ScanKey() == 8) { 
                Display_7seg(pass);
            } else {
                Display_7seg(input);
            }
        }
    }
}
