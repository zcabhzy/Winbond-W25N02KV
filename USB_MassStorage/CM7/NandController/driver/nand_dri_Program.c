/*
 *  nand_dri_Program.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_Program.h"

/* ===========================================================================
 * Function: LoadProgramData
 * ===========================================================================
 * @brief
 *  - Executes LOAD PROGRAM DATA (02h) to write data into the cache register.
 *
 * @details
 *  - Loads a data segment from host to the NAND internal cache.
 *  - Typically used before ProgramExecute(10h) to program data into array.
 *  - Transfers main data area (0x0000 ~ 0x07FF) or spare area
 *    (0x0800 ~ 0x083F) depending on column address.
 *
 *  Command Sequence:
 *    [02h] + [CA1:CA0] + [Data...]
 *      - CA: 2-byte column address within the page
 *
 * @param col_addr : Column start address within the page.
 * @param buf      : Pointer to data buffer to be programmed.
 * @param len      : Number of bytes to load into cache.
 *
 * @note
 *  - Follow with ProgramExecute(10h) to commit data to NAND array.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.8
 * --------------------------------------------------------------------------- */
void LoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len)
{
	uint8_t command[3];

	command[0] = CMD_LOAD_PROGRAM_DATA;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = col_addr & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 3);
	HAL_SPI_TX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: RandomLoadProgramData
 * ===========================================================================
 * @brief
 *  - Executes RANDOM DATA INPUT (84h) to update specific bytes in cache.
 *
 * @details
 *  - Enables partial overwrite of cache contents without reloading full page.
 *  - Commonly used to modify spare area or partial page data.
 *
 *  Command Sequence:
 *    [84h] + [CA1:CA0] + [Data...]
 *
 * @param col_addr : Column start address for random input.
 * @param buf      : Pointer to data buffer to be programmed.
 * @param len      : Number of bytes to load into cache.
 *
 * @note
 *  - Must follow LoadProgramData(02h) and precede ProgramExecute(10h).
 *  - Reference: Winbond W25N02KV Datasheet 8.2.8
 * --------------------------------------------------------------------------- */
void RandomLoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len)
{
	uint8_t command[3];

	command[0] = CMD_RANDOM_LOAD_PROGRAM_DATA;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = col_addr & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 3);
	HAL_SPI_TX(buf, len);
	CS_HIGH();
}

void QuadLoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len)
{
	uint8_t command[3];

	command[0] = CMD_QUAD_LOAD_PROGRAM_DATA;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = col_addr & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 3);
	HAL_SPI_TX(buf, len);
	CS_HIGH();
}

void QuadRandomLoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len)
{
	uint8_t command[3];

	command[0] = CMD_QUAD_RANDOM_LOAD_PROGRAM_DATA;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = col_addr & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 3);
	HAL_SPI_TX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: ProgramExecute
 * ===========================================================================
 * @brief
 *  - Executes PROGRAM EXECUTE (10h) to write cache data into NAND array.
 *
 * @details
 *  - Commits the data loaded in cache to the target physical page.
 *  - Requires WriteEnable(06h) before issuing this command.
 *  - Program operation time (tPROG) typically 200–600 µs.
 *  - Host must monitor SR[0] (OIP) or R/B# pin until ready.
 *
 *  Command Sequence:
 *    [10h] + [PA2:PA1:PA0] (3-byte page address)
 *
 * @param page_addr : 24-bit page address (Block/Page combined).
 *
 * @note
 *  - After completion, check SR3[3] (P_FAIL) to verify success.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.10
 * --------------------------------------------------------------------------- */
void ProgramExecute(uint32_t page_addr)
{
	uint8_t command[4];

	command[0] = CMD_PROGRAM_EXECUTE;
	command[1] = (page_addr >> 16) & 0xFF;
	command[2] = (page_addr >> 8) & 0xFF;
	command[3] = page_addr & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 4);
	CS_HIGH();
}
