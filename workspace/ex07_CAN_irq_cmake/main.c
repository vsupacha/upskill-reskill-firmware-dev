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
#include "stm32c5xx_hal_fdcan.h"
#include "stm32c5xx_hal_gpio.h"

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
      HAL_GPIO_TogglePin(HAL_GPIOA, HAL_GPIO_PIN_5);
    }
  }
} /* end main */

// RXFIFO0 callback
void HAL_FDCAN_RxFifo0Callback(hal_fdcan_handle_t *hfdcan, uint32_t rx_fifo0_interrupts) {
  if ((rx_fifo0_interrupts & HAL_FDCAN_FLAG_RX_FIFO_0_NEW_MSG) != RESET) {
    
  }
}

// RXFIFO1 callback
void HAL_FDCAN_RxFifo0Callback(hal_fdcan_handle_t *hfdcan, uint32_t rx_fifo0_interrupts) {

}