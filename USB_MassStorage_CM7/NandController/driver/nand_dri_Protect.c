/*
 *  nand_dri_Protect.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_Protect.h"

/* ===========================================================================
 * Function: WriteEnable
 * ===========================================================================
 * @brief
 *  - Executes WRITE ENABLE (06h) to set the Write Enable Latch (WEL) bit.
 *
 * @details
 *  - Allows subsequent program or erase operations.
 *  - The WEL bit (SR[1]) must be set to "1" before issuing any write, erase,
 *    or protection register modification command.
 *  - The bit automatically clears (WEL=0) after completion of a program or
 *    erase cycle, or after power-on reset.
 *
 * @note
 *  - Does not affect any data or status content.
 *  - Should be verified by reading Status Register SR[1] = 1.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.5
 * --------------------------------------------------------------------------- */
void WriteEnable(void)
{
	uint8_t command = WRITE_ENABLE;

	CS_LOW();
	HAL_SPI_TX(&command, 1);
	CS_HIGH();
}

/* ===========================================================================
 * Function: WriteDisable
 * ===========================================================================
 * @brief
 *  - Executes WRITE DISABLE (04h) to clear the Write Enable Latch (WEL) bit.
 *
 * @details
 *  - Prevents any further program or erase operations until WriteEnable(06h)
 *    is reissued.
 *  - Used to protect memory array and registers from unintended modification.
 *
 * @note
 *  - Clears SR[1] (WEL = 0).
 *  - Recommended after maintenance or test operations.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.6
 * --------------------------------------------------------------------------- */
void WriteDisable(void)
{
	uint8_t command = WRITE_DISABLE;

	CS_LOW();
	HAL_SPI_TX(&command, 1);
	CS_HIGH();
}
