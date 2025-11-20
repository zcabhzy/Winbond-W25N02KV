/*
 *  nand_hal.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *
 *  Address: NandController/hal
 */

#include "nand_hal.h"

/* -------------------------------------------------------------------------
 * @brief
 *
 * HAL_SPI_RX
 *
 *
 * HAL_SPI_TX
 * ------------------------------------------------------------------------- */
void HAL_SPI_TX(const uint8_t *data, uint16_t len)
{
	HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);
}

/* -------------------------------------------------------------------------
 * Function Introduction
 * -------------------------------------------------------------------------
 * HAL_SPI_RX
 *
 *
 * HAL_SPI_TX
 * ------------------------------------------------------------------------- */
void HAL_SPI_RX(uint8_t *data, uint16_t len)
{
	HAL_SPI_Receive(&hspi2, data, len, HAL_MAX_DELAY);
}
