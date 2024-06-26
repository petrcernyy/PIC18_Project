#ifndef UART_H
#define	UART_H

void UART_main(void);

void UART_Init(void);

void uart_write_norm(char *text, int len);

void uart_write(char *text, int len);

void putch(char data);


#endif	/* UART_H */

