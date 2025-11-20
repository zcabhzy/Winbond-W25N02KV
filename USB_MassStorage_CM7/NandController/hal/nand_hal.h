/*
 *  nand_hal.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *
 *  Address: NandController/hal
 */

#ifndef HAL_NAND_HAL_H_
#define HAL_NAND_HAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "stm32h7xx_hal.h"

extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart3;

#define CS_LOW()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

/* -------------------------------------------------------------------------
 * Function Introduction
 * -------------------------------------------------------------------------
 * HAL_SPI_RX
 *
 *
 * HAL_SPI_TX
 * ------------------------------------------------------------------------- */
void HAL_SPI_RX(uint8_t *data, uint16_t len);        // Standard SPI Recevice
void HAL_SPI_TX(const uint8_t *data, uint16_t len);  // Standard SPI Send

#endif /* HAL_NAND_HAL_H_ */
