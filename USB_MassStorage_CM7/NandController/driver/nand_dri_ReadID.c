/*
 *  nand_dri_ReadID.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_ReadID.h"

/* ===========================================================================
 * Function: W25N02_JEDECID
 * ===========================================================================
 * @brief
 *  - Executes READ JEDEC ID (9Fh) to retrieve manufacturer and device IDs.
 *
 * @details
 *  - Transmits [9Fh] command to request identification data from the device.
 *  - Response : [Manufacturer ID][Device ID][Capacity]
 *
 * @param buf: Pointer to 3-byte RX buffer for storing JEDEC ID data.
 *
 * @note
 *  - Reference: Winbond W25N02KV Datasheet 8.2.2
 * --------------------------------------------------------------------------- */
void W25N02_JEDECID(uint8_t *buf)
{
	if (buf == NULL)
		return;

	uint8_t command = JEDECID;
	uint8_t dummy = 0x00;

	CS_LOW();
	HAL_SPI_TX(&command, 1);
	HAL_SPI_TX(&dummy, 1);
	HAL_SPI_RX(buf, 3);
	CS_HIGH();
}
