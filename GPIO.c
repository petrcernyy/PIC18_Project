#include "GPIO.h"
#include <xc.h>
#include "struct.h"
#include "lcd.h"
#include "ISR.h"

extern volatile button_debounc buttons;         //struct pro ovladani debouncingu

void GPIO_main(void){
    
    LCD_ShowString(1,"Rozsvecovani LED ");
    LCD_ShowString(2,"Stiskni BTN2     ");
    
    char ledky = 0b111111;
    driveLED(ledky);
    int i = 0;
    
    while(1){
        
        if(buttons.btn2_state){
            if(i>5){
                i = 0;
                ledky = 0b111111;
                driveLED(ledky);
            }
            else{
                ledky ^= 1<<i;
                driveLED(ledky);
                i++;
            }
            buttons.btn2_state = 0;
        }
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            char in = 0b111111;
            driveLED(in);
            return;
        }
        
    }
    
}

void driveLED(char in){
    LATD2 = in & 1;             //LED0
    LATD3 = in & 2 ? 1 : 0;     //LED1
    LATC4 = in & 4 ? 1 : 0;     //LED2
    LATD4 = in & 8 ? 1 : 0;     //LED3
    LATD5 = in & 16 ? 1 : 0;    //LED4
    LATD6 = in & 32 ? 1 : 0;    //LED5
}
