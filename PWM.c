#include <xc.h>
#include <stdio.h>
#include "PWM.h"
#include "struct.h"
#include "lcd.h"
#include "ISR.h"

extern volatile button_debounc buttons;         //struct pro ovladani debouncingu

void PWM_main(void){
    
    PWM_Init();
    
    char txt[17];
    int jas = 0;
    int duty = 199;
    
    LCD_ShowString(1,"BTN2 prepina jas   ");
    sprintf(txt,"Jas:  %d perc   ", jas);
    LCD_ShowString(2,txt);
    
    while(1){
        
        if(buttons.btn2_state){
            buttons.btn2_state = 0;
            jas = jas + 20;
            duty = duty - 39;
            if(jas > 100){
                jas = 0;
            }
            
            if(duty < 0){
                CCPR1L = 199;
                duty = 199;
            }
            else{
                CCPR1L = duty;
            }
            sprintf(txt,"Jas:  %d perc   ", jas);
            LCD_ShowString(2,txt);
        }
        
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            TRISDbits.TRISD5 = 0;
            PSTR1CONbits.STR1B = 0;
            TMR4ON = 0;
            return;
        }
    }
}

void PWM_Init(void){
    
    TRISDbits.TRISD5 = 1;
    PSTR1CONbits.STR1A = 0;
    PSTR1CONbits.STR1B = 1;
    
    CCPTMRS0bits.C1TSEL = 0b01;
    
    PR4 = 199;
    
    CCP1CONbits.CCP1M = 0b1100;
    CCP1CONbits.P1M = 0b00;
    
    CCPR1L = 199;
    
    T4CONbits.T4CKPS = 0b01;
    TMR4IF = 0;
    TMR4ON = 1;
    
    while(!TMR4IF);
    
    TRISDbits.TRISD5 = 0;
    
    
}
