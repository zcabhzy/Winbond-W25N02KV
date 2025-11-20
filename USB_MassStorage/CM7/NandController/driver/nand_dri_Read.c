/*
 *  nand_dri_Read.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_Read.h"

/* ===========================================================================
 * Function: PageDataRead
 * ===========================================================================
 * @brief
 *  - Executes PAGE DATA READ (13h) to load one page from array into cache.
 *
 * @details
 *  - Transfers one full page (main + spare) from NAND array to cache.
 *  - Host must ensure OIP = 0 in Status Register (SR[0]) before
 *    issuing the next command.
 *
 * Command Sequence:
 *    [13h] + [PA2:PA1:PA0] (3-byte page address)
 *
 * Next Step:
 *    ReadData(03h) or FastRead(0Bh) to fetch cache contents.
 *
 * @param page_addr: 24-bit page address (Block/Page combined).
 *
 * @note
 *  - Reference: Winbond W25N02KV Datasheet 8.2.11
 * --------------------------------------------------------------------------- */
void PageDataRead(uint32_t page_addr)
{
	uint8_t command[4];

	command[0] = CMD_PAGE_DATA_READ;
	command[1] = (page_addr >> 16) & 0xFF;
	command[2] = (page_addr >> 8) & 0xFF;
	command[3] = (page_addr >> 0) & 0xFF;

	CS_LOW();
	HAL_SPI_TX(command, 4);
	CS_HIGH();
}

/* ===========================================================================
 * Function: ReadData
 * ===========================================================================
 * @brief
 *  - Executes READ DATA (03h) to read cache contents through single I/O.
 *
 * @details
 *  - Transfers data from the internal cache register to the host via SPI bus.
 *  - Ensure OIP = 0 (SR[0]) before send command.
 *  - Commonly used after PageDataRead(13h) to output cached page data.
 *
 * Command Sequence:
 *  - CA: 2-byte column address within the page (0x0000 ~ 0x07FF)
 *  - Dummy: 1 byte (8 dummy clocks)
 *
 * Flow:
 *  -> PageDataRead[13h]: Transfer data to cache
 *  -> ReadData[03h]: Fetch data from cache.
 *
 * @param col_addr :  Column start address within the page.
 * @param buf      :  Read data buffer
 * @param len      :  Number of bytes to read.
 *
 * @note
 *  - Reference: Winbond W25N02KV Datasheet 8.2.12
 * --------------------------------------------------------------------------- */
void ReadData(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_READ_DATA;
	command[1] = (col_addr >> 8) & 0xFF; /// CA[15:8]
	command[2] = (col_addr >> 0) & 0xFF; /// CA[7:0]
	command[3] = 0x00;					 /// Dummy byte (8 clocks)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastRead
 * ===========================================================================
 * @brief
 *  - Executes FAST READ (0Bh) to read cache data with higher SPI frequency.
 *
 * @details
 *  - Similar to READ DATA (03h) but includes one dummy byte to support higher
 *    SPI clock rates.
 *  - Must ensure OIP = 0 (SR[0]) before issuing this command.
 *
 * Command Sequence:
 *  - CA: 2-byte column address within the page (0x0000 ~ 0x07FF)
 *  - Dummy: 1 byte (8 dummy clocks)
 *
 * Flow:
 *  -> PageDataRead(13h): Transfer page data into cache
 *  -> FastRead(0Bh): Read data from cache at higher SPI speed
 *
 * @param col_addr : Column start address within the page.
 * @param buf      : Pointer to data buffer for storing read data.
 * @param len      : Number of bytes to read.
 *
 * @note
 *  - Reference: Winbond W25N02KV Datasheet 8.2.13
 * --------------------------------------------------------------------------- */
void FastRead(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadWith4ByteAddr
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_WITH_4BYTE_ADDRESS;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF:1 -> 24 clocks | BUF:0 -> 40 clocks)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadDualOutput
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadDualOutput(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_DUAL_OUTPUT;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;   // Dummy (BUF=1: 8 cycles, BUF=0: 32 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadDualOutputWith4ByteAddr
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadDualOutputWith4ByteAddr(uint16_t col_addr, uint8_t *buf,
		uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_DUAL_OUTPUT_4BYTE;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;   // Dummy (BUF=1: 24 cycles, BUF=0: 40 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadQuadOutput
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadQuadOutput(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_QUAD_OUTPUT;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;   // Dummy (BUF=1: 8 cycles, BUF=0: 32 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadQuadOutputWith4ByteAddr
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadQuadOutputWith4ByteAddr(uint16_t col_addr, uint8_t *buf,
		uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_QUAD_OUTPUT_4BYTE;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF=1: 24 cycles, BUF=0: 40 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadDualIO
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadDualIO(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_DUAL_IO;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF=1: 4 cycles, BUF=0: 16 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadDualIOWith4ByteAddr
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadDualIOWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_DUAL_IO_4BYTE;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF=1: 12 cycles, BUF=0: 20 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadQuadIO
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadQuadIO(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_QUAD_IO;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF=1: 4 cycles, BUF=0: 12 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

/* ===========================================================================
 * Function: FastReadQuadIOWith4ByteAddr
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 * --------------------------------------------------------------------------- */
void FastReadQuadIOWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	uint8_t command[4];

	command[0] = CMD_FAST_READ_QUAD_IO_4BYTE;
	command[1] = (col_addr >> 8) & 0xFF;
	command[2] = (col_addr >> 0) & 0xFF;
	command[3] = 0x00;  // Dummy (BUF=1: 10 cycles, BUF=0: 14 cycles)

	CS_LOW();
	HAL_SPI_TX(command, 4);
	HAL_SPI_RX(buf, len);
	CS_HIGH();
}

