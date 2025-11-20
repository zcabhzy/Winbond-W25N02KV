/*
 *  nand_dri_ReadID.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#ifndef DRIVER_NAND_READID_H_
#define DRIVER_NAND_READID_H_

#include "nand_hal.h"

/* ---------------------------------------------------------------------------
 * Command Code Definition
 * ---------------------------------------------------------------------------
 * JEDECID : 9Fh
 * --------------------------------------------------------------------------- */
#define JEDECID 0x9F

/* ---------------------------------------------------------------------------
 * Command driver
 * ---------------------------------------------------------------------------
 * W25N02_JEDECID
 *  - Read Winbond W25N02KV JEDEC ID
 * --------------------------------------------------------------------------- */
void W25N02_JEDECID(uint8_t *buf);

#endif /* DRIVER_NAND_READID_H_ */
