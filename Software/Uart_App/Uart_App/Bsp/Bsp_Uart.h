#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f1xx.h"
#include "stdio.h"

#define UART_STRING 0		//�ַ������淽ʽ


#define USART_REC_LEN 200  //�����������ֽ���

#define RXBUFFERSIZE	1	


extern UART_HandleTypeDef huart1;

int fputc(int ch,FILE *f);
int fgetc(FILE *f);


#endif /* __BSP_UART_H*/
