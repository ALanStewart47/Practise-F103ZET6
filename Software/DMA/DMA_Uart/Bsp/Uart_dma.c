#include "Uart_dma.h"



//uint8_t uart_send_buff[111];

/**
 * @brief  Usart configuration
 * @note   None
 * @retval None
 */
 #if 0
void Bsp_UartConfig(void)
{
	// usart gpio struct
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// usart gpio clk enable
	BSP_UART_DMA_CLK_ENABLE();
	BSP_UART_DMA_GPIO_CLK_ENABLE();
	
	//RX GPIO Initializes
	GPIO_InitStruct.Pin=BSP_UART_DMA_RX_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(BSP_UART_DMA_RX_GPIO_PORT,&GPIO_InitStruct);
	
	//TX GPIO Initializes
	GPIO_InitStruct.Pin=BSP_UART_DMA_TX_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BSP_UART_DMA_TX_GPIO_PORT,&GPIO_InitStruct);
	
	UartHandle.Instance					=		BSP_UART_DMA;
	UartHandle.Init.BaudRate		=		BSP_USART_DMA_BAUDRATE;
	UartHandle.Init.WordLength	=		UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits		=		UART_STOPBITS_1;
	UartHandle.Init.Parity			=		UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl		=		UART_HWCONTROL_NONE;
	UartHandle.Init.Mode				=		UART_MODE_TX_RX;
	
	HAL_UART_Init(&UartHandle);
}
#endif


/**
 * @brief  Dma configuration
 * @note   None
 * @retval None
 */
#if 0
void DMA_UART_Config(void)
{
	BSP_DMA_UART_CLK_ENABLE();

	//Register base address
	DmaHandle.Instance=BSP_UART_DMA_CHANNEL;


	DmaHandle.Init.Mode									=		DMA_NORMAL;							//the operation mode of the DMAy Channelx
	DmaHandle.Init.Direction						=		DMA_MEMORY_TO_PERIPH;
	DmaHandle.Init.MemDataAlignment			=		DMA_MDATAALIGN_BYTE;		//the Memory data width.  
	DmaHandle.Init.PeriphDataAlignment	=		DMA_PDATAALIGN_BYTE;		//the peripheric data width
	DmaHandle.Init.MemInc								=		DMA_MINC_ENABLE;				//whether the memory address register should be incremented or not
	DmaHandle.Init.PeriphInc						=		DMA_PINC_DISABLE;				//外设增量模式：disable
	DmaHandle.Init.Priority							=		DMA_PRIORITY_MEDIUM;		//优先级

	HAL_DMA_Init(&DmaHandle);
	
	__HAL_LINKDMA(&UartHandle,hdmatx,DmaHandle);
}
#endif


int fputc(int ch,FILE *f)
{
	//send 1 byte data to uart, in dma mode, dma send data to uart 
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,1000);
	
	return (ch);
}


int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&huart1,(uint8_t *)&ch,1,1000);
	
	return ch;
}


