/*
 *  ReadID_service.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "ReadID_service.h"

/* ===========================================================================
 * Function: Print_JEDECID
 * ===========================================================================
 * @brief
 *  - Reads and displays the JEDEC ID of the connected W25N02KV NAND device.
 *
 * @details
 *  - Issues the READ JEDEC ID (9Fh) command sequence to obtain the device identity.
 *  - The response consists of three bytes:
 *        Byte[0] = Manufacturer ID (Winbond → 0xEF)
 *        Byte[1] = Memory Type     (W25N family → 0xAA)
 *        Byte[2] = Device Capacity (2Gb device → 0x22)
 *  - Used for hardware bring-up, SPI verification, and device detection at boot.
 *
 *  Command Flow:
 *    1. [9Fh] → Issue JEDEC ID command
 *    2. Read 3-byte response from the device
 *    3. Print result via UART/console
 * --------------------------------------------------------------------------- */
void Print_JEDECID()
{
	uint8_t id[3] = { 0 };

	W25N02_JEDECID(id);
	printf("[Read JEDEC ID] : %02X %02X %02X\r\n", id[0], id[1], id[2]);
}
