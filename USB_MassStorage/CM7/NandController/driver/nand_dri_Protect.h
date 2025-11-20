/*
 *  nand_dri_Protect.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_PROTECT_H_
#define DRIVER_NAND_DRI_PROTECT_H_

#include "nand_hal.h"

#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04

void WriteEnable(void);
void WriteDisable(void);

#endif /* DRIVER_NAND_DRI_PROTECT_H_ */
