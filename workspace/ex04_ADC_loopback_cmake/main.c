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
#include "mx_adc1.h"
#include "mx_dac1.h"
#include "mx_usart2.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_adc.h"
#include "stm32c5xx_hal_dac.h"
#include "stm32c5xx_hal_uart.h"
#include <stdint.h>

#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
uint16_t gen_voltage(void);
uint16_t read_voltage(void);
void report_voltage(uint16_t dac_val, uint16_t adc_val);

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
    HAL_DAC_StartChannel(mx_dac1_gethandle(), HAL_DAC_CHANNEL_1);
    HAL_ADC_Start(mx_adc1_gethandle());
    while (1) {
      HAL_GPIO_WritePin(HAL_GPIOA, HAL_GPIO_PIN_5, HAL_GPIO_PIN_SET); // LED on
      uint16_t dac_val = gen_voltage();
      uint16_t adc_val = read_voltage();
      report_voltage(dac_val, adc_val);
      HAL_GPIO_WritePin(HAL_GPIOA, HAL_GPIO_PIN_5, HAL_GPIO_PIN_RESET); // LED off
      HAL_Delay(1000); 
    }
  }
} /* end main */

// generate voltage on DAC channel 1
uint16_t gen_voltage(void) {
  static uint16_t dac_val = 0;
  dac_val = (dac_val + 100)%4000;
  HAL_DAC_SetChannelData(mx_dac1_gethandle(), HAL_DAC_CHANNEL_1, dac_val);
  return dac_val;
}

// read voltage from ADC channel 0
uint16_t read_voltage(void) {
  HAL_ADC_REG_StartConv(mx_adc1_gethandle());
  HAL_ADC_REG_PollForConv(mx_adc1_gethandle(), 10);
  return HAL_ADC_REG_ReadConversionData(mx_adc1_gethandle());
}

// report voltage by printing out the DAC and ADC values
void report_voltage(uint16_t dac_val, uint16_t adc_val) {
  char str[64];
  int len = sprintf(str, "DAC: %d, ADC: %d\n", dac_val, adc_val);
  HAL_UART_Transmit(mx_usart2_uart_gethandle(), str, len, 100);
}