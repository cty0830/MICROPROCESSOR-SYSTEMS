//
// GPIO_7seg_keypad : 3x3 keypad input and display on 7-segment LEDs
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"

// 宣告全域變數做為中斷旗標 (0: 正常碼表模式, 1: 跑馬燈模式)
int marquee_mode = 0;

// 用來備份進入跑馬燈前的碼表狀態 (mode)
int backup_mode = 0; 

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

void EINT1_IRQHandler(void)
{
    GPIO_CLR_INT_FLAG(PB, BIT15);   
    
    if (marquee_mode == 0) {
        marquee_mode = 1;           // 碼表進入跑馬燈模式
    }
}

void Init_EXTINT(void)
{
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);
    GPIO_EnableEINT1(PB, 15, GPIO_INT_RISING); 
    NVIC_EnableIRQ(EINT1_IRQn);

    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);
}

int main(void)
{
    int time = 0;
    int mode = 0; // 0: 初始狀態, 1: 開始計時, 2: 暫停/停止, 3: 重置, 4: 加50秒
    int minute = 0;
    int sec = 0;
    int count = 0;

    uint16_t i;
    uint16_t old = 0; // 用於儲存上次的按鍵狀態

    // 跑馬燈相關變數 (對應你新增的 16 ~ 21 腳位編碼)
    // #define SEG_N16  0xF7
    // #define SEG_N17  0xEF
    // #define SEG_N18  0xFE
    // #define SEG_N19  0xDF
    // #define SEG_N20  0xBF
    // #define SEG_N21  0xFB
    // uint8_t SEG_BUF[22]={SEG_N0, SEG_N1, SEG_N2, SEG_N3, SEG_N4, SEG_N5, SEG_N6, SEG_N7, SEG_N8, SEG_N9, SEG_N10, SEG_N11, SEG_N12, SEG_N13, SEG_N14, SEG_N15, SEG_N16, SEG_N17, SEG_N18, SEG_N19, SEG_N20, SEG_N21}; 

    int current_seg = 16;  
    int marquee_count = 0; 
    
    // 狀態追蹤旗標：用來標記這次進入跑馬燈時，是否已經備份過碼表狀態
    int is_backed_up = 0;

    SYS_Init();
    OpenSevenSegment();
    OpenKeyPad();
    Init_EXTINT();         // 啟用 PB15 中斷功能

    while(1) {
        i = ScanKey();

        // 【放開按鍵才觸發邏輯】
        if(i == 0 && old != 0) {
            
            // 如果目前在跑馬燈模式，放開任意按鍵立刻解鎖
            if (marquee_mode == 1) {
                marquee_mode = 0;      // 退出跑馬燈模式
                mode = backup_mode;    // 先還原備份的計時狀態
                is_backed_up = 0;      // 重設備份標記
                
                // 如果解鎖跑馬燈時放開的剛好是功能鍵，直接覆蓋並執行該功能
                if(old == 1) {
                    mode = 1; 
                } else if(old == 2) {
                    mode = 2;
                } else if(old == 3) { 
                    mode = 2;          // 【核心修正】從跑馬燈切回時若按 3，強制設為暫停狀態
                    time = 0; 
                    count = 0; 
                } else if(old == 4) {
                    time += 50;
                }
            } 
            else {
                // 正常碼表模式下的按鍵邏輯
                if(old == 1){
                    mode = 1; // 開始計時
                }else if(old == 2){
                    mode = 2; // 暫停計時
                }else if(old == 3){
                    mode = 2; // 【核心修正】按下 3 歸零時，強制將狀態改為 2 (暫停/停止)，不能繼續跑
                    time = 0;
                    count = 0;
                }else if(old == 4){
                    time += 50; // 加時 50 秒
                }
            }
        }
        old = i; // 紀錄本次按鈕狀態

        // ==========================================
        // 狀態分支 1：中斷觸發的跑馬燈模式
        // ==========================================
        if (marquee_mode == 1) {
            // 在剛進入跑馬燈的當下，把現在的計時狀態（mode）備份起來
            if (is_backed_up == 0) {
                backup_mode = mode; 
                is_backed_up = 1;
            }
            
            // 直接呼叫底層 API 來驅動 16~21 跑馬燈
            CloseSevenSegment(); ShowSevenSegment(3, current_seg); CLK_SysTickDelay(5000); 
            CloseSevenSegment(); ShowSevenSegment(2, current_seg); CLK_SysTickDelay(5000); 
            CloseSevenSegment(); ShowSevenSegment(1, current_seg); CLK_SysTickDelay(5000); 
            CloseSevenSegment(); ShowSevenSegment(0, current_seg); CLK_SysTickDelay(5000); 

            marquee_count++;
            if (marquee_count >= 5) { 
                current_seg++;
                if (current_seg > 21) current_seg = 16; 
                marquee_count = 0;
            }
        }
        // ==========================================
        // 狀態分支 2：正常的碼表計時與顯示模式
        // ==========================================
        else {
            minute = time / 60;
            sec = time % 60;
            
            Display_7seg(minute * 100 + sec);

            // 【核心修正】只有在 mode == 1 (計時中) 的情況下才增加時間，其餘狀態 (如暫停、歸零) 一律不計時
            if(mode == 1){
                count++;
                if(count >= 50){ // 50 * 0.02s = 1.0 秒
                    time++;
                    count = 0;
                }
            }
        }
    }
}
