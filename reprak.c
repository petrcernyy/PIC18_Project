#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "ISR.h"
#include "reprak.h"
#include "reprak.h"
#include "struct.h"
#include "notes.h"
#include "lcd.h"

extern volatile button_debounc buttons;

volatile reprak_control reprak;

void reprak_main(void){
    
    reprak_Init();
    
    LCD_ShowString(2,"                  ");
    LCD_ShowString(1,"BTN1: START/STOP  ");

    char btn1 = 0;
    char txt[17] = "                 ";
    int len;
    int perc;
    
    
    while(1){
        
        if(reprak.counter == 0){
            strcpy(txt,"                 ");
            LCD_ShowString(2,txt);
        }
        
        if(buttons.btn1_state){
            buttons.btn1_state = 0;
            btn1 ^= 1;
            if(btn1){
                TMR3ON = 1;
                TMR3IE = 1;
                TMR4ON = 1;
                TMR4IE = 1;
            }
            else{
                TMR3ON = 0;
                TMR3IE = 0;
                TMR4ON = 0;
                TMR4IE = 0;
            }
        }
        
        if(reprak.flag && btn1){
            
            reprak.freq = ((1000000/melody[reprak.counter])/2);
            len = melody[reprak.counter+1];
            if(len > 0){
                reprak.full_note = 1600/len;
            }
            else if(len < 0){
                reprak.full_note = 1600/len;
                reprak.full_note = reprak.full_note * -1.5;
            }
            
            perc = (reprak.counter/((float)reprak.array_len))*16;
            txt[perc] = 219;
            LCD_ShowString(2,txt);
            
            
            reprak.flag = 0;
        }
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            TMR3ON = 0;
            TMR3IE = 0;
            TMR4ON = 0;
            TMR4IE = 0;
            return;
        }
    }
}

void reprak_Init(void){
    
    T4CONbits.T4CKPS = 0b01;
    T4CONbits.T4OUTPS = 0b0111;
    TMR4ON = 0;
    TMR4IE = 0;
    TMR4IF = 0;
    PR4 = 250;

    T3CONbits.TMR3CS = 0b00;
    T3CONbits.T3CKPS = 0b11;
    
    TMR3ON = 0;
    TMR3IE = 0;
    TMR3IF = 0;
    
    PEIE = 1;
    GIE = 1;
    
    reprak.counter = 0;
    reprak.flag = 1;
    reprak.array_len = sizeof(melody)/sizeof(melody[0]);

    TRISBbits.TRISB5 = 0;
    
    
}
