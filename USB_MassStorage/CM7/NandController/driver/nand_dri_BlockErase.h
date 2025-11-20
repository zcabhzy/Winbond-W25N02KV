/*
 *  nand_dri_BlockErase.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_BLOCKERASE_H_
#define DRIVER_NAND_DRI_BLOCKERASE_H_

#include "nand_hal.h"

void BlockErase128KB(uint32_t block_addr);

#endif /* DRIVER_NAND_DRI_BLOCKERASE_H_ */
