#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "ADC.h"
#include "struct.h"
#include "lcd.h"
#include "ISR.h"

#define _XTAL_FREQ 32e6

extern volatile button_debounc buttons;

void ADC_main(void){
    
    ADC_Init();
    
    uint16_t data_pot1;
    uint16_t data_pot2;
    
    float pot1;
    float pot2;
    
    char txt_pot1[17];
    char txt_pot2[17];
    
    char change = 1;
    
    while(1){
        
        GODONE = 1;
        while(GODONE);
        
        if(change){
            data_pot1 = (uint16_t)(ADRESH << 8) | ADRESL;
            pot1 = (data_pot1/(float)1023)*3.3;
            ADCON0bits.CHS = 5;
        }
        else{
            data_pot2 = (uint16_t)(ADRESH << 8) | ADRESL;
            pot2 = (data_pot2/(float)1023)*3.3;
            ADCON0bits.CHS = 4;
        }
        
        change ^= 1;
        
        sprintf(txt_pot1,"POT1:  %.1f V    ", pot1);
        sprintf(txt_pot2,"POT2:  %.1f V    ", pot2);
        LCD_ShowString(1,txt_pot1);
        LCD_ShowString(2,txt_pot2);
        
        if(buttons.btn4_state){
            ADON = 0;
            buttons.btn4_state = 0;
            return;
        }
        
    }
}

void ADC_Init(void){
    
    ANSELAbits.ANSA5 = 1;
    ANSELEbits.ANSE0 = 1;
    
    TRISAbits.TRISA5 = 1;
    TRISEbits.TRISE0 = 1;
    
    ADCON2bits.ADCS = 0b010;
    ADCON2bits.ACQT = 0b110;
    ADCON2bits.ADFM = 1;
    ADCON1bits.PVCFG = 0b00;
    ADCON1bits.NVCFG = 0b00;
    ADCON0bits.CHS = 4;
    ADON = 1;
    
    
    
}