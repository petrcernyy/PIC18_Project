#include <xc.h>
#include <string.h>
#include "UART.h"
#include "struct.h"
#include "lcd.h"
#include "ISR.h"

extern volatile button_debounc buttons;         //struct pro ovladani debouncingu

void UART_main(void){
    
    UART_Init();
    
    LCD_ShowString(1,"      UART       ");
    LCD_ShowString(2," Otoceni zpravy  ");
    
    char text[30];          //Retezec pro ukladani zpravy
    char idx = 0;
    char flag = 0;          //Flag pro prijmuti zpravy
    
    char uvod[90] = "Periferie UART posle zpravu, o maximalni delce 30 znaku, zpet v opacnem poradi.\n";
    
    uart_write_norm(&uvod, 90);
    
    while(1){
        
        //Ukladani zpravy
        if(RC1IF){
            if(RCREG1 == '\n'){
                text[idx] = '\n';
                idx = 0;
                flag = 1;
            }
            else{
                text[idx] = RCREG1;
                idx++;
            }
        }
        
        //Otoceni zpravy a odeslani
        if(flag){
            uart_write(&text,30);
            memset(text, 0, sizeof text);
            flag = 0;
        }
        
        if(buttons.btn4_state){
            buttons.btn4_state = 0;
            return;
        }
    }
    
    
}

void UART_Init(void){
    
    ANSELC = 0x00;
    
    TRISCbits.TRISC6 = 1;       //TX a RX pin
    TRISCbits.TRISC7 = 1;
    
    SPBRG1 = 25;            //Baudrate 19200
    
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TXEN = 1;
    RCSTA1bits.SPEN = 1;
    RCSTA1bits.CREN = 1;
 
    return;
}

void uart_write_norm(char *text, int len){
    int i;
    for(i = 0; i <= len; i++){
        if(text[i] == '\n'){
            putch(text[i]);
            return;
        }
        else{
            putch(text[i]);
        }
    }
}

void uart_write(char *text, int len){
    int i;
    char flag = 0;
    for(i = len; i >= 0; i--){
        if(text[i] == '\n'){
            flag = 1;
        }
        else if(flag){
            putch(text[i]);
        }
    }
    putch('\n');
    putch('\n');
}

void putch(char data){
    while(!TX1IF){
        continue;
    }
    TXREG1 = data;
}