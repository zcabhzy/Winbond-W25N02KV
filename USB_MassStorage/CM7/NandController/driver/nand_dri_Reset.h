/*
 *  nand_dri_Reset.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_DRI_RESET_H_
#define DRIVER_NAND_DRI_RESET_H_

#include "nand_hal.h"

/* ---------------------------------------------------------------------------
 * Command Code Definition
 * ---------------------------------------------------------------------------
 * RESET        : 99h
 * ENABLE_RESET : 66h
 * DEVICE_RESET : FFh
 * --------------------------------------------------------------------------- */
#define RESET 0x99
#define ENABLE_RESET 0x66
#define DEVICE_RESET 0xFF

/* ---------------------------------------------------------------------------
 * Command driver
 * ---------------------------------------------------------------------------
 * Reset       : Send 66h command to enable reset
 * EnableReset : Send 99h command to perform software reset
 * DeviceReset : Send FFh command to perform full device reset
 * --------------------------------------------------------------------------- */
void Reset(void);
void EnableReset(void);
void DeviceReset(void);

#endif /* DRIVER_NAND_DRI_RESET_H_ */
