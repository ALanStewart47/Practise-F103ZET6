#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "main.h"

//#define SPI_CS_Low()  HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,0)
//#define SPI_CS_Hight() HAL_GPIO_WritePin(SPI_NS_GPIO_Port,SPI_NS_Pin,1)

/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID                       0xEF3015     //W25X16
//#define  sFLASH_ID                       0xEF4015	    //W25Q16
#define  sFLASH_ID                        0XEF4017     //W25Q64
//#define  sFLASH_ID                       0XEF4018    //W25Q128

//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256


/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			      0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			    0xC7 
#define W25X_PowerDown			    0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF


/*等待超时时间*/
#define SPI_TransmitReceive_Timeout 1000
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)



extern SPI_HandleTypeDef hspi1;
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);

uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
uint8_t SPI_FLASH_ReadByte(void);

void SPI_FLASH_WriteEnable();
void SPI_FLASH_WriteDisable();
void SPI_FLASH_WaitForWriteEnd();

void SPI_FLASH_SectorErase(uint32_t SectorAdrr);
void SPI_FLASH_ChipErase(uint32_t SectorErase);

void SPI_FLASH_PageWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);

void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

#endif /*__BSP_FLASH */
