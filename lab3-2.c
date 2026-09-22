#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"

int number = 0;     
int count = 0;      
void Display_7seg()
{
    uint8_t digit;
    int value = number;

    if (count == 0){
        CloseSevenSegment();
        return;
    }

    if (count >= 4){
        digit = value / 1000;

        CloseSevenSegment();
        ShowSevenSegment(3, digit);

        CLK_SysTickDelay(1000);
    }

    if (count >= 3){
        digit = (value / 100) % 10;

        CloseSevenSegment();
        ShowSevenSegment(2, digit);

        CLK_SysTickDelay(1000);
    }

    if (count >= 2){
        digit = (value / 10) % 10;

        CloseSevenSegment();
        ShowSevenSegment(1, digit);

        CLK_SysTickDelay(1000);
    }

    if (count >= 1){
        digit = value % 10;

        CloseSevenSegment();
        ShowSevenSegment(0, digit);

        CLK_SysTickDelay(1000);
    }
}

void Push(int n){
    if (count < 4){
        number = number * 10 + n;
        count++;
    }
}

void Pop(void){
    if (count > 0){
        number = number / 10;
        count--;

        if (count == 0){
            number = 0;
            CloseSevenSegment();
        }
    }
}

int main(void){
    uint16_t key;

    SYS_Init();
    OpenSevenSegment();
    OpenKeyPad();
    CloseSevenSegment();
	
    while (1){
        Display_7seg();
        key = ScanKey();

        if (key != 0){
            switch (key){
                case 1:
                    Push(1);
                    break;

                case 2:
                    Push(2);
                    break;

                case 3:
                    Push(3);
                    break;

                case 4:
                    Push(4);
                    break;

                case 5:
                    Push(5);
                    break;

                case 6:
                    Push(6);
                    break;

                case 7:
                    Pop();
                    break;

                default:
                    break;
            }

            while (ScanKey() != 0){
                Display_7seg();
            }
        }
    }
}
