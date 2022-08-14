#include "bsp_flash.h"



uint8_t SPI_FLASH_SendByte(uint8_t byte)
{

    //Define two arrays to hold the RX and TX buffers.
    uint8_t RXD[1];
    uint8_t TXD[1];

    //Get the data of byte
    TXD[0] = byte;

    if(HAL_SPI_TransmitReceive(&hspi1, TXD, RXD, 1,SPI_TransmitReceive_Timeout)!=HAL_OK)
    {
        return HAL_ERROR;
    }
    return RXD[0];

}

uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  //SPI_FLASH_CS_LOW();
	HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,0);

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(0x9F);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(0xFF);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(0xFF);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(0xFF);

  /* 停止通讯：CS高电平 */
  //SPI_FLASH_CS_HIGH();
	HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,1);

	/*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}



