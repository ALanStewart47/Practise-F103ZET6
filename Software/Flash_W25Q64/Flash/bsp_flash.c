#include "bsp_flash.h"

/*Initializes the Flash interface and the Systick. */

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
extern SPI_HandleTypeDef hspi1;

/**
 * @brief  SPI CS Low level
 * @note   
 * @retval None
 */
void SPI_CS_Low()
{
  HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,0);
}

/**
 * @brief  SPI CS High level
 * @note   
 * @retval None
 */
void SPI_CS_High()
{
  HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,1);
}


/**
 * @brief  send a byte to W25X_ Flash 
 * @note   
 * @param  byte: data
 * @retval received data of W25X_ Flash 
 */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{

    //Define two arrays to hold the RX and TX buffers.
    uint8_t RXD[1];
    uint8_t TXD[1];

    //Get the data of byte
    TXD[0] = byte;

    if(HAL_SPI_TransmitReceive(&hspi1, TXD, RXD, 1,SPIT_FLAG_TIMEOUT)!=HAL_OK)
    {
        return HAL_ERROR;
    }
    return RXD[0];
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (__HAL_SPI_GET_FLAG( &hspi1,  SPI_FLAG_TXE ) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }

  /* Send Half Word through the SPIx peripheral */
  WRITE_REG(hspi1.Instance->DR, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (__HAL_SPI_GET_FLAG( &hspi1, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* Return the Half Word read from the SPI bus */
  return READ_REG(hspi1.Instance->DR);
}



/**
 * @brief  read a byte from W25X_ Flash
 * @note   
 * @retval return a byte
 */

uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}



/**
 * @brief  :The Write Enable must be set prior to every Page Program, Sector Erase, Block Erase, Chip Erase and
Write Status Register instruction. 
 * @note   :this function is entered by driving /CS low, shifting the
instruction code “06h” into the Data Input (DI) pin on the rising edge of CLK, and then driving /CS high.
 * @retval 
 */
void SPI_FLASH_WriteEnable()
{
  //driving /CS low,
  SPI_CS_Low();
  //shifting the instruction code “06h” into the (DI) pin on the rising edge of CLK
  SPI_FLASH_SendByte(W25X_WriteEnable);
    //driving /CS High
  SPI_CS_High();
}


/**
 * @brief  Disables the SPI write function.
 * @note   
 * @retval None
 */
void SPI_FLASH_WriteDisable()
{
  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_WriteDisable);

  SPI_CS_High();
}

/**
 * @brief  check the program, erase or write whether completed 
 * @note   :BUSY is a read only bit in the status register (S0) that is set to a 1 state when the device is executing a
Page Program, Sector Erase, Block Erase, Chip Erase or Write Status Register instruction
 * @retval 
 */
void SPI_FLASH_WaitForWriteEnd()
{
  uint8_t FLASH_Status=0;

  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  while((FLASH_Status & WIP_Flag) != 0)
  {
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
  }
  
  SPI_CS_High();
}



/**
 * @brief  read FLASH ID
 * @note   
 * @retval flash id
 */

uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  //SPI_FLASH_CS_LOW();
	SPI_CS_Low();

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  SPI_CS_High();

	/*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}


/**
 * @brief  Read Flash device id 
 * @note   
 * @retval Flash ID
 */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
    uint32_t Temp = 0;

  /* 开始通讯：CS低电平 */
  //SPI_FLASH_CS_LOW();
	SPI_CS_Low();

  /* 发送指令，读取Device ID */
  SPI_FLASH_SendByte(W25X_DeviceID);

  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);

  /*Read Flash device ID*/
  Temp= SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  SPI_CS_High();

  return Temp;
}



/**
 * @brief  Erase Flash Sector 
 * @note   
 * @param  byte: 
 * @retval 
 */
void SPI_FLASH_SectorErase(uint32_t SectorAdrr)
{
  //ENABLE SPI Write
  SPI_FLASH_WriteEnable();
  //Wait for other operate to flash
  SPI_FLASH_WaitForWriteEnd();

  /* 开始通讯：CS低电平 */
  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_SectorErase);
  
  SPI_FLASH_SendByte((SectorAdrr & 0xff0000)>>16);
  SPI_FLASH_SendByte((SectorAdrr & 0xff00)>>8);
  SPI_FLASH_SendByte(SectorAdrr & 0xff);

  /* 停止通讯：CS高电平 */
  SPI_CS_High();
  //Wait for other operate to flash
  SPI_FLASH_WaitForWriteEnd();
}


/**
 * @brief  Erase FLASH Chip
 * @note   
 * @param  byte: 
 * @retval 
 */
void SPI_FLASH_ChipErase(uint32_t SectorErase)
{
  //ENABLE SPI Write
  SPI_FLASH_WriteEnable();
  //Wait for other operate to flash
  SPI_FLASH_WaitForWriteEnd();

  /* 开始通讯：CS低电平 */
  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_ChipErase);

  /* 停止通讯：CS高电平 */
  SPI_CS_High();
  //Wait for other operate to flash
  SPI_FLASH_WaitForWriteEnd();
}


/**
 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
 * @note   
 * @param  pBuffer: 
 * @param  WriteAddr: 
 * @param  NumByteToWrite: 
 * @retval None
 */
void SPI_FLASH_PageWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
  SPI_FLASH_WriteEnable();

  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_PageProgram);

  SPI_FLASH_SendByte((WriteAddr & 0xff0000)>>16);

  SPI_FLASH_SendByte((WriteAddr & 0xff00)>>8);

  SPI_FLASH_SendByte(WriteAddr & 0xff);

  if(NumByteToWrite>SPI_FLASH_PageSize)
  {
    NumByteToWrite=SPI_FLASH_PageSize;
    FLASH_ERROR("Flash pagewrite too large!");
  }

  while(NumByteToWrite--)
  {
      SPI_FLASH_SendByte(*pBuffer);
      pBuffer++;
  }

  SPI_CS_High();
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  write data to flash 
 * @note   
 * @param  pBuffer: 
 * @param  WriteAddr: 
 * @param  NumByteToWrite: 
 * @retval None
 */

void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
  uint8_t NumOfPage=0,NumOfSingle = 0,
  Addr = 0,count=0,temp=0;

  //判断写入地址是否与页刚好对齐
  Addr = WriteAddr % SPI_FLASH_PageSize;

  //差count个数据值，刚好可以对齐到页地址
  count = SPI_FLASH_PageSize - Addr;

  //需要烧写的整数页数，即不包含不满一页内容的数据
  NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
  //不满一页内容数据的个数
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if(Addr == 0)
  {
    if(NumOfPage == 0)
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else
    {
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
      SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
    }
  }
  /* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else
  {
    //写入的数据少于一页大小（256byte）
    if(NumOfPage == 0)
    {
      //写入的数据超过地址剩余的数据
      if(NumOfSingle > count)
      {
        //剩余的数据个数
        temp = NumOfSingle - count;

        //写入数据，先写完一页
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer+= count;

        //写入剩余的数据
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else  //写入的数据大小能放在当前页中 
      {
         SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }

    }
    //写入的数据大于一页的大小
    else
    {
      //减掉与页地址不对齐的数据，然后计算占用页、和剩余数据
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize; 

      //写入开头不满一页的数据
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr += count;
      pBuffer+= count;

      //写入页数据
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr,SPI_FLASH_PageSize);
        pBuffer += SPI_FLASH_PageSize;
        WriteAddr += SPI_FLASH_PageSize;

      }

      //写入剩余不足一页的数据
      if(NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


/**
 * @brief  read Flash data 
 * @note   
 * @param  pBuffer: data buffers
 * @param  ReadAddr: data address
 * @param  NumByteToRead: data numbers 
 * @retval None
 */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  
  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_ReadData);


  SPI_FLASH_SendByte((ReadAddr & 0xff0000)>>16);

  SPI_FLASH_SendByte((ReadAddr & 0xff00)>>8);

  SPI_FLASH_SendByte(ReadAddr & 0xff);

  while(NumByteToRead--)
  {
    //读取一个字节
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    //
    pBuffer ++;
  }

  SPI_CS_High();
}



/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select Flash :chip select low*/
  SPI_CS_Low();

  SPI_FLASH_SendByte(W25X_ReadData);
  
  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}



//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* 选择 FLASH: CS 低 */
  SPI_CS_Low();

  /* 发送 掉电 命令 */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* 停止信号  FLASH: CS 高 */
  SPI_CS_High();
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  SPI_CS_Low();

  /* 发上 上电 命令 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* 停止信号 FLASH: CS 高 */
  SPI_CS_High();                   //等待TRES1
}   


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}


/************************************END OF FILE*********************************************/

