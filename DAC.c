#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "DAC.h"
#include "ISR.h"
#include "struct.h"
#include "UART.h"
#include "lcd.h"

//Jumper na ANALOG OUT 2 - RB0

extern volatile button_debounc buttons;

#define _XTAL_FREQ 32e6
#define SS LATBbits.LATB3
#define mess 0b00110000

void DAC_main(void){
    
    DAC_Init();
    LCD_ShowString(1,"  Trojuhelnik   ");
    LCD_ShowString(2,"POT1:Amp,POT2:Hz");
    
    char i = 0;
    char dir = 1;
    uint16_t data_pot1;
    uint16_t data_pot2;
    int A = 255;
    int freq = 1;

    
    while(1){

        if(TMR1 >= freq){
            TMR1 = 0;
        
            ADCON0bits.CHS = 12;
            DAC_Write(i);
            if(i >= A) dir = 0;
            else if(i <= 0) dir = 1;
            if(dir) i++;
            else if(!dir) i--;
            GODONE = 1;
            while(GODONE);
            printf("%d, \r",((ADRESH<<8)|ADRESL));
        }
        
        ADCON0bits.CHS = 4;
        GODONE = 1;
        while(GODONE);
        data_pot1 = (uint16_t)(ADRESH<<8) | ADRESL;
        A = (data_pot1/(float)1023)*(255-50)+50;
        
        ADCON0bits.CHS = 5;
        GODONE = 1;
        while(GODONE);
        data_pot2 = (uint16_t)(ADRESH<<8) | ADRESL;
        freq = (data_pot2/(float)1023)*(10000 - 10) + 10;
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            TMR1ON = 0;
            SSP1CON1bits.SSPEN = 0;
            ADCON0bits.ADON = 0;
            RCSTA1bits.CREN = 0;
            TXSTA1bits.TXEN = 0;
            
            return;
        }
    }
    
}

void DAC_Init(void){
    
    //Nastaveni SPI
    ANSELC = 0x00;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    TRISBbits.TRISB3 = 0;
    
    SS = 1;
    
    SSP1CON1bits.SSPM = 0b1010;
    
    SSP1ADD = 199;
    
    SSP1STATbits.CKE = 1;
    
    SSP1CON1bits.SSPEN = 1;
    
    //Nastaveni ADC
    ANSELBbits.ANSB0 = 1;
    ANSELAbits.ANSA5 = 1;
    ANSELEbits.ANSE0 = 1;
    
    TRISAbits.TRISA5 = 1;
    TRISEbits.TRISE0 = 1;
    
    ADCON2bits.ADCS = 0b110;
    ADCON0bits.CHS = 4;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b110;
    
    ADCON0bits.ADON = 1;
    
    //Nastaveni UART
    SPBRG1 = 51;
    
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    
    RCSTA1bits.SPEN = 1;
    RCSTA1bits.CREN = 1;
    TXSTA1bits.TXEN = 1;
   
    //Nastaveni TMR1
    T1CONbits.TMR1CS = 0b00;
    T1CONbits.T1CKPS = 0b11;
    TMR1ON = 1;
    
    TMR1 = 0;
    
    
}

void DAC_Write(uint8_t data){
    
    uint8_t msb, lsb;
    msb = mess | (data>>4);
    lsb = (data<<4) & 0xF0;
    
    SS = 0;
    
    PIR1bits.SSPIF = 0;
    
    SSPBUF = msb;
    while(!SSP1IF);
    
    PIR1bits.SSPIF = 0;   
    SSPBUF = lsb;
    while(!SSP1IF);
    
    SS = 1;
    
}