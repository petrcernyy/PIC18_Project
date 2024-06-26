#include "ISR.h"
#include <xc.h>
#include "struct.h"
#include "GAME.h"
#include "reprak.h"

extern volatile button_debounc buttons;     //struct pro ovladani buttonu
extern volatile reprak_control reprak;      //struct pro ovladani prehravace

#define BTN1 PORTCbits.RC0
#define BTN2 PORTAbits.RA4
#define BTN3 PORTAbits.RA3
#define BTN4 PORTAbits.RA2
#define RAISING_EDGE 0b01111111
#define delay 0xFFFF - 1000             //delay pro GAME - odhad casu

void __interrupt() ISR_HANDLER(void){
    
    //Ovladani Debouncingu
    if(TMR2IF && TMR2IE){
        
        buttons.btn1_acc <<= 1;
        buttons.btn1_acc |= BTN1;
        
        buttons.btn2_acc <<= 1;
        buttons.btn2_acc |= BTN2;
        
        buttons.btn3_acc <<= 1;
        buttons.btn3_acc |= BTN3;
        
        buttons.btn4_acc <<= 1;
        buttons.btn4_acc |= BTN4;
        
        if(buttons.btn1_acc == RAISING_EDGE){
            buttons.btn1_state = 1;
        }
        else if(buttons.btn1_acc){
            buttons.btn1_state = 0;
        }
        if(buttons.btn2_acc == RAISING_EDGE){
            buttons.btn2_state = 1;
        }
        else if(buttons.btn2_acc){
            buttons.btn2_state = 0;
        }
        if(buttons.btn3_acc == RAISING_EDGE){
            buttons.btn3_state = 1;
        }
        else if(buttons.btn3_acc){
            buttons.btn3_state = 0;
        }
        if(buttons.btn4_acc == RAISING_EDGE){
           buttons.btn4_state = 1; 
        }
        else if(buttons.btn4_acc){
            buttons.btn4_state = 0;
        }
        
        TMR2IF = 0;
    }

    //Interrupt pro prehravac
    static int t_c2 = 0;
    
    //ovladani noty
    if(TMR3IE && TMR3IF){
        LATBbits.LATB5 ^= 1;
        TMR3 = 0xFFFF - reprak.freq;
        TMR3IF = 0;
    }
    //ovladani delky noty
    if(TMR4IF && TMR4IE){
        if(t_c2 > reprak.full_note){
            reprak.counter = reprak.counter + 2;
            if(reprak.counter > reprak.array_len){
                reprak.counter = 0;
            }
            t_c2 = 0;
            reprak.flag = 1;
        }
        t_c2++;
        TMR4 = 0;
        TMR4IF = 0;
    }
    

} 