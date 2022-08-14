#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "stm32f1xx_hal.h"
#include "main.h"

#define SPI_TransmitReceive_Timeout 1000

extern SPI_HandleTypeDef hspi1;


uint32_t SPI_FLASH_ReadID(void);

#endif /*__BSP_FLASH */
