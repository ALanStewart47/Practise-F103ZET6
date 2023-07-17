#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f1xx.h"
#include "stdio.h"

#define UART_STRING 0		//字符串储存方式


#define USART_REC_LEN 200  //定义最大接收字节数

#define RXBUFFERSIZE	1	


extern UART_HandleTypeDef huart1;

int fputc(int ch,FILE *f);
int fgetc(FILE *f);


#endif /* __BSP_UART_H*/
