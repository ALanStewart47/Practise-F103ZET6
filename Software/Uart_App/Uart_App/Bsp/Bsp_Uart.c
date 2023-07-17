#include "Bsp_Uart.h"



#if UART_STRING
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
#endif /* __UART_STRING_H_ */

#if 1
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
uint8_t USART_RX_BUF[USART_REC_LEN];	//接收缓冲数组
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;		//接收状态标记

uint8_t aRxBuffer[RXBUFFERSIZE];	//HAL库使用的串口接收缓冲

void HAL_UART_RxCpltCallback(UART_HandleTypeDef	* huart)
{
	if(huart->Instance==USART1)
	{
		if((USART_RX_STA&0x8000)==0)		//接收未完成
		{
			if(USART_RX_STA&0x4000)				//接收到0x0d
			{
					if(aRxBuffer[0]!=0x0a)
						USART_RX_STA=0;					//接收错误,重新开始
					else 
						USART_RX_STA|=0x8000;		//接收完成
			}
			else  //没收到0x0d
			{
				if(aRxBuffer[0]==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0x3FFF]=aRxBuffer[0];
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;
				}
				
			}
		}
	}
}
void USART1_IRQHandler(void)
{
	uint32_t timeout=0;
	uint32_t maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&huart1);
	
	timeout=0;
	while(HAL_UART_GetState(&huart1)!=HAL_UART_STATE_READY)
	{
		timeout++;
		if(timeout>maxDelay)
			break;
	}
	
	timeout =0;
	while(HAL_UART_Receive_IT(&huart1,(uint8_t *)aRxBuffer,RXBUFFERSIZE)!=HAL_OK)
	{
		timeout++;
		if(timeout>maxDelay)
			break;
	}
}




#endif

