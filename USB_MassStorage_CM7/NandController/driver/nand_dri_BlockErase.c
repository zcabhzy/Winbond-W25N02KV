/*
 *  nand_dri_BlockErase.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_BlockErase.h"

/* ===========================================================================
 * Function: BlockErase128KB
 * ===========================================================================
 * @brief
 *  - Execute BLOCK ERASE (D8h) to erase one 128-KB block in the NAND array.
 *
 * @details
 *  - Erases all pages within the specified block (typically 64 pages × 2 KB).
 *  - Requires WriteEnable(06h) before issuing this command.
 *  - During erase, the device is busy (OIP = 1 in SR[0]).
 *  - After completion, host must check SR[2] (E_FAIL) for erase-fail detection.
 *
 *  Command Sequence:
 *    [D8h] + [BA2:BA1:BA0] (3-byte block address)
 *      - BA: block address aligned to 128-KB boundary
 *
 * @param block_addr : 24-bit block address (Block index × 64 pages).
 *
 * @note
 *  - Ensure target block is not marked bad before erasing.
 *  - Recommended flow:
 *       1. WriteEnable(06h)
 *       2. BlockErase128KB(D8h)
 *       3. Poll SR[0] (OIP = 0)
 *       4. Verify SR[2] (E_FAIL = 0)
 *  - Reference: Winbond W25N02KV Datasheet 8.2.7
 * --------------------------------------------------------------------------- */
void BlockErase128KB(uint32_t block_addr)
{
	uint8_t command[4];

	command[0] = 0xD8;
	command[1] = ((block_addr >> 16) & 0xFF);
	command[2] = ((block_addr >> 8) & 0xFF);
	command[3] = ((block_addr >> 0) & 0xFF);

	CS_LOW();
	HAL_SPI_TX(command, 4);
	CS_HIGH();
}
