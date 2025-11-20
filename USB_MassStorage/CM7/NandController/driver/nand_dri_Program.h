/*
 *  nand_dri_Program.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_PROGRAM_H_
#define DRIVER_NAND_DRI_PROGRAM_H_

#include "nand_hal.h"

#define CMD_LOAD_PROGRAM_DATA             0x02
#define CMD_RANDOM_LOAD_PROGRAM_DATA      0x84
#define CMD_QUAD_LOAD_PROGRAM_DATA        0x32
#define CMD_QUAD_RANDOM_LOAD_PROGRAM_DATA 0x34
#define CMD_PROGRAM_EXECUTE               0x10

void LoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len);
void RandomLoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len);
void QuadLoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len);
void QuadRandomLoadProgramData(uint16_t col_addr, const uint8_t *buf,
		uint16_t len);
void ProgramExecute(uint32_t page_addr);

#endif /* DRIVER_NAND_DRI_PROGRAM_H_ */
