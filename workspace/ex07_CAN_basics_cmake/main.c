/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mx_fdcan1.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_fdcan.h"
#include "stm32c5xx_hal_gpio.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }
  else
  {
    /*
      * You can start your application code here
      */
    HAL_FDCAN_Start(mx_fdcan1_gethandle());
    HAL_GPIO_WritePin(HAL_GPIOE, HAL_GPIO_PIN_2, HAL_GPIO_PIN_RESET);
    while (1) {
      uint8_t buf[64];
      hal_fdcan_tx_header_t fdcan_header = {
        .b.identifier = 0x01,
        .b.frame_type = HAL_FDCAN_FRAME_DATA,
        .b.identifier_type = HAL_FDCAN_ID_STANDARD,
        .b.data_length = HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE,
        .b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN
      };
      memset(buf,1,64);

      HAL_FDCAN_ReqTransmitMsgFromFIFOQ(mx_fdcan1_gethandle(), &fdcan_header, buf);
      HAL_Delay(1000);
    }
  }
} /* end main */

