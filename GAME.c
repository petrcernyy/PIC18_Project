#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "GAME.h"
#include "lcd.h"
#include "ISR.h"

extern volatile button_debounc buttons;

void GAME_main(void){
    
    GAME_Init();
    
    int vysledek;
    int cas;
    char game_end = 1;
    int tmr_c;
    
    char txt_g[17];
    
    LCD_ShowString(1,"  Hadani casu    ");
    LCD_ShowString(2,"  Stiskni BTN1   ");
    
    while(1){

        if(buttons.btn1_state && game_end){
            game_end = 0;
            cas = (rand() % 8) + 3;
            sprintf(txt_g,"       %d s         ", cas);
            LCD_ShowString(1,"Zmackni BTN1 po:  ");
            LCD_ShowString(2,txt_g);
            TMR1ON = 1;
            tmr_c = 0;
            buttons.btn1_state = 0;   
        }
        
        if(buttons.btn1_state && !game_end){
            TMR1ON = 0;
            TMR1 = 0;
            vysledek = (cas*1000) - tmr_c;
            sprintf(txt_g,"Rozdil: %d ms", vysledek);
            LCD_ShowString(2,"Nova hra BTN1    ");
            LCD_ShowString(1,txt_g);
            game_end = 1;
            buttons.btn1_state = 0;
        }
        if(TMR1 >= 1000){
            TMR1 = 0;
            tmr_c++;
            game_end = 0;
        }
        
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            return;
        }
    }
}

void GAME_Init(void){
    
    T1CONbits.TMR1CS = 0b00;
    T1CONbits.T1CKPS = 0b11;
    TMR1ON = 0;
    
    TMR1 = 0;
    
}