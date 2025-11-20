/*
 *  nand_dri_StatusRegister.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_STATUSREGISTER_H_
#define DRIVER_NAND_DRI_STATUSREGISTER_H_

#include "nand_hal.h"

#define READ_SR  0x05
#define WRITE_SR 0x01

uint8_t ReadStatusRegister(uint8_t sr_addr);
void WriteStatusRegister(uint8_t sr_addr, uint8_t value);

#endif /* DRIVER_NAND_DRI_STATUSREGISTER_H_ */
