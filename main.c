/*
238671
    0:GPIO - BTN2 rozsvecuje postupne LED
    1:UART - Poslani retezce naopak
    2:PWM - BTN2 prepíná jas LED mezi peti úrovnemi
    3:ADC - Vypis pot1 a pot2 na displej ve V                     
    4:DAC - Generator trojuhelniku
    5:GAME - Odhad casu
    6:TIMER - Prehravac hudby
 */

#pragma config FOSC = HSMP        // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = ON         // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config PRICLKEN = ON          // Primary clock enable bit (Primary clock is always enabled)
#pragma config WDTEN = OFF         // watchdog off

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "ISR.h"
#include "lcd.h"
#include "struct.h"
#include "GPIO.h"
#include "UART.h"
#include "GAME.h"
#include "ADC.h"
#include "PWM.h"
#include "reprak.h"
#include "DAC.h"

#define _XTAL_FREQ 32e6

volatile button_debounc buttons = {0};

void Init(void);

void main(void) {
    
    char flag = 1;
    Init();                 //Pocatecni nastaveni
    int menu_kurz = 0;      //Counter pro vyber v menu
    char in = 0b111111;     //Ovladani LED
    driveLED(in);
    LCD_Init();
    
    while(1){
        //Prepinani radku v menu
        if(buttons.btn2_state){
            flag = 1;
            menu_kurz++;
            if(menu_kurz > 6){
                menu_kurz = 6;
            }
            buttons.btn2_state = 0;
        }
        if(buttons.btn1_state){
            flag = 1;
            menu_kurz--;
            if(menu_kurz < 0){
                menu_kurz = 0;
            }
            buttons.btn1_state = 0;
        }
        if(buttons.btn3_state){
            flag = 1;
        }
        
        //Zobrazeni menu
        if(flag){
        switch(menu_kurz){
            case 0:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    GPIO_main();
                }
                LCD_ShowString(1,"> 1 - GPIO        ");
                LCD_ShowString(2,"  2 - UART        ");
                flag = 0;
                break;
            case 1:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    UART_main();
                }
                LCD_ShowString(1,"  1 - GPIO         ");
                LCD_ShowString(2,"> 2 - UART         ");
                flag = 0;
                break;
            case 2:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    PWM_main();
                }
                LCD_ShowString(1,"> 3 - PMW         ");
                LCD_ShowString(2,"  4 - ADC         ");
                flag = 0;
                break;
            case 3:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    ADC_main();
                }
                LCD_ShowString(1,"  3 - PMW         ");
                LCD_ShowString(2,"> 4 - ADC         ");
                flag = 0;
                break;
            case 4:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    DAC_main();
                }
                LCD_ShowString(1,"> 5 - DAC         ");
                LCD_ShowString(2,"  6 - GAME         ");
                flag = 0;
                break;
            case 5:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    GAME_main();
                }
                LCD_ShowString(1,"  5 - DAC         ");
                LCD_ShowString(2,"> 6 - GAME         ");
                flag = 0;
                break;
            case 6:
                if(buttons.btn3_state){
                    buttons.btn3_state = 0;
                    reprak_main();
                }
                LCD_ShowString(1,"> 7 - PREHRAVAC         ");
                LCD_ShowString(2,"                    ");
                flag = 0;
                break;
                
        }
        }
        
    }
    
    return;
}

void Init(void){
    
    TRISD = 0b10000011;     //LED output
    TRISCbits.TRISC4 = 0;
    
    ANSELA = 0x00;          //Analog vypnut pro tlacitka
    
    TRISCbits.TRISC0 = 1;       //BTN1-4 input
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA2 = 1;
    
    //TIMER2 pro debouncing
    T2CONbits.T2CKPS = 0b11;       //Prescaler 1:16 
    T2CONbits.T2OUTPS = 0b1111;     //Postscaler 1:16
    PR2 = 250;                      //Hodnota interruptu 8ms
    TMR2IF = 0;
    TMR2IE = 1;
    PEIE = 1;
    TMR2ON = 1;
    GIE = 1;
    
}