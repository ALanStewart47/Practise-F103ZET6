#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f1xx.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;

int fputc(int ch,FILE *f);
int fgetc(FILE *f);


#endif /* __BSP_UART_H*/
