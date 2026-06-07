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
#include "app_config.h"
#include "mx_usart2.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_gpio.h"
#include "stm32c5xx_hal_uart.h"
#include "scpi.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char uart_buf[1];
volatile scpi_status_t scpi_status = SCPI_STATUS_IDLE;

/* Private functions prototype -----------------------------------------------*/
void HAL_UART_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event) {
  scpi_status = SCPI_Parse_Char(uart_buf[0]);
  HAL_UART_Receive_IT(mx_usart2_uart_gethandle(), uart_buf, 1);
}

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
    HAL_UART_Receive_IT(mx_usart2_uart_gethandle(), uart_buf, 1);
    while (1) {
      static char cmd_resp[MAX_SCPI_BUF_SIZE];
      static char error_msg[MAX_SCPI_BUF_SIZE];

      switch(scpi_status) {
        case SCPI_STATUS_READY:
          SCPI_Execute(cmd_resp);
          HAL_UART_Transmit_IT(mx_usart2_uart_gethandle(), cmd_resp, strlen(cmd_resp));
          scpi_status = SCPI_STATUS_IDLE;
          break;
        case SCPI_STATUS_ERROR:
          SCPI_Error(scpi_status, error_msg);
          HAL_UART_Transmit_IT(mx_usart2_uart_gethandle(), error_msg, strlen(cmd_resp));
          scpi_status = SCPI_STATUS_IDLE;
        case SCPI_STATUS_IDLE:
          // do nothing
          break;
        default:
          // do nothing?
          scpi_status = SCPI_STATUS_IDLE;
      }
      HAL_Delay(1);
    }
  }
} /* end main */

