/*
 *  nand_dri_Read.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_READ_H_
#define DRIVER_NAND_DRI_READ_H_

#include "nand_hal.h"

/* ---------------------------------------------------------------------------
 * Command Code Definition
 * ---------------------------------------------------------------------------
 * CMD_PAGE_DATA_READ           	 : 13h -> Transfer Page to Cache
 * CMD_READ_DATA 					 : 03h -> Read data from cache
 * CMD_FAST_READ 	                 : 0Bh
 * CMD_FAST_READ_WITH_4BYTE_ADDRESS  : 0Ch
 * CMD_FAST_READ_DUAL_OUTPUT 		 : 3Bh
 * CMD_FAST_READ_DUAL_OUTPUT_4BYTE   : 3Ch
 * CMD_FAST_READ_QUAD_OUTPUT         : 6Bh
 * CMD_FAST_READ_QUAD_OUTPUT_4BYTE   : 6Ch
 * CMD_FAST_READ_DUAL_IO             : BBh
 * CMD_FAST_READ_DUAL_IO_4BYTE       : BCh
 * CMD_FAST_READ_QUAD_IO             : EBh
 * CMD_FAST_READ_QUAD_IO_4BYTE       : ECh
 * --------------------------------------------------------------------------- */
#define CMD_PAGE_DATA_READ           	  0x13
#define CMD_READ_DATA 					  0x03
#define CMD_FAST_READ 	                  0x0B
#define CMD_FAST_READ_WITH_4BYTE_ADDRESS  0x0C
#define CMD_FAST_READ_DUAL_OUTPUT 		  0x3B
#define CMD_FAST_READ_DUAL_OUTPUT_4BYTE   0x3C
#define CMD_FAST_READ_QUAD_OUTPUT         0x6B
#define CMD_FAST_READ_QUAD_OUTPUT_4BYTE   0x6C
#define CMD_FAST_READ_DUAL_IO             0xBB
#define CMD_FAST_READ_DUAL_IO_4BYTE       0xBC
#define CMD_FAST_READ_QUAD_IO             0xEB
#define CMD_FAST_READ_QUAD_IO_4BYTE       0xEC

/* ---------------------------------------------------------------------------
 * Command driver
 * ---------------------------------------------------------------------------
 * PageDataRead
 * ReadData
 * FastRead
 * FastReadWith4ByteAddr
 * FastReadDualOutput
 * FastReadDualOutputWith4ByteAddr
 * FastReadQuadOutput
 * FastReadQuadOutputWith4ByteAddr
 * FastReadDualIO
 * FastReadDualIOWith4ByteAddr
 * FastReadQuadIO
 * FastReadQuadIOWith4ByteAddr
 * --------------------------------------------------------------------------- */
void PageDataRead(uint32_t page_addr);
void ReadData(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastRead(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadDualOutput(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadDualOutputWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadQuadOutput(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadQuadOutputWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadDualIO(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadDualIOWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadQuadIO(uint16_t col_addr, uint8_t *buf, uint16_t len);
void FastReadQuadIOWith4ByteAddr(uint16_t col_addr, uint8_t *buf, uint16_t len);

#endif /* DRIVER_NAND_DRI_READ_H_ */
