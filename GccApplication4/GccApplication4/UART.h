/*
 * UART.h
 *
 * Created: 24/02/2023 4:53:12 PM
 *  Author: BLU-RAY
 */ 
void UART_init(long USART_BAUDRATE);
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);
void UART_SendNbr(int N);