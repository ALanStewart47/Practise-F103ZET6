#ifndef __UART_DMA_H
#define __UART_DMA_H

#include <stdint.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

/*****************************************************************/
#define BSP_UART_DMA											USART1
#define BSP_USART_DMA_BAUDRATE						115200


#define BSP_UART_DMA_CLK_ENABLE()					__HAL_RCC_USART1_CLK_ENABLE();
#define BSP_UART_DMA_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();


#define BSP_UART_DMA_RX_PIN								GPIO_PIN_10
#define BSP_UART_DMA_RX_GPIO_PORT					GPIOA
#define BSP_UART_DMA_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();

#define BSP_UART_DMA_TX_PIN								GPIO_PIN_9
#define BSP_UART_DMA_TX_GPIO_PORT					GPIOA
#define BSP_UART_DMA_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();


/*	DMA		*/
#define BSP_DMA_UART_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE();
#define BSP_UART_DMA_CHANNEL					DMA1_Channel4

/*	extern init UART & DMA*/
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;


UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef DmaHandle;


#endif /* __UART_DMA_H_ */

