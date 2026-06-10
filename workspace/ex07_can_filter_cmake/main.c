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
#include "mx_usart2.h"
#include "stm32c5xx_hal_fdcan.h"
#include "stm32c5xx_hal_gpio.h"
#include "stm32c5xx_hal_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TXT_BUF_SIZE  64

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
      static uint32_t msg_count = 0;
      hal_fdcan_rx_header_t fdcan_header;
      uint8_t payload[TXT_BUF_SIZE];
      char printout[TXT_BUF_SIZE];
      int pout_len = 0;

      HAL_FDCAN_GetRxFifoFillLevel(mx_fdcan1_gethandle(), HAL_FDCAN_RX_FIFO_0, &msg_count);
      if (msg_count > 0) {
        HAL_FDCAN_GetReceivedMessage(mx_fdcan1_gethandle(), HAL_FDCAN_RX_FIFO_0, &fdcan_header, payload);
        pout_len = snprintf(printout, TXT_BUF_SIZE, "RX0:%d - %d\n", fdcan_header.b.identifier, fdcan_header.b.data_length);
        HAL_UART_Transmit_IT(mx_usart2_uart_gethandle(), printout, pout_len);
      }
      HAL_FDCAN_GetRxFifoFillLevel(mx_fdcan1_gethandle(), HAL_FDCAN_RX_FIFO_1, &msg_count);
      if (msg_count > 0) {
        HAL_FDCAN_GetReceivedMessage(mx_fdcan1_gethandle(), HAL_FDCAN_RX_FIFO_1, &fdcan_header, payload);
        pout_len = snprintf(printout, TXT_BUF_SIZE, "RX1:%d - %d\n", fdcan_header.b.identifier, fdcan_header.b.data_length);
        HAL_UART_Transmit_IT(mx_usart2_uart_gethandle(), printout, pout_len);
      }
      HAL_Delay(1000);
    }
  }
} /* end main */

