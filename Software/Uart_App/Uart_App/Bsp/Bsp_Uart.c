#include "Bsp_Uart.h"




int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,1000);
	
	return (ch);
}

int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&huart1,(uint8_t *)&ch,1,1000);
	return (ch);
}


void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	uint8_t ch=0;
	
	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET)
	{
		ch=	(uint16_t)READ_REG(huart1.Instance->DR);
		WRITE_REG(huart1.Instance->DR,ch);
	}
  /* USER CODE END USART1_IRQn 0 */
  //HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}