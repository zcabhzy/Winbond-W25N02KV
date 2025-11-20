/*
 *  nand_dri_StatusRegister.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_StatusRegister.h"

/* ===========================================================================
 * Function: ReadStatusRegister
 * ===========================================================================
 * @brief
 *  - Executes READ STATUS REGISTER (0Fh) to read status or configuration data.
 *
 * @details
 *  - Sends [0Fh] followed by the status register address byte (SRx).
 *  - Returns one byte of data from the selected status register.
 *  - Typical address mapping:
 *       SR1 (0xA0): Operation status (OIP, WEL, ECCS0~1)
 *       SR2 (0xB0): Program/Erase fail flags
 *       SR3 (0xC0): Feature control (QE, ECC enable, OTP lock)
 *  - Common usage includes polling OIP (SR1[0]) or verifying ECC/FAIL bits.
 *
 *  Command Sequence:
 *    [0Fh] + [SR_Addr] + [Data_Out]
 *
 * @param sr_addr : Target status register address (e.g., 0xA0, 0xB0, 0xC0).
 *
 * @return uint8_t : Value of the selected status register.
 *
 * @note
 *  - Safe to execute at any time; no dependency on OIP bit.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.3
 * --------------------------------------------------------------------------- */
uint8_t ReadStatusRegister(uint8_t sr_addr)
{
	uint8_t command[2] = { READ_SR, sr_addr };
	uint8_t sr = 0;

	CS_LOW();
	HAL_SPI_TX(command, 2);
	HAL_SPI_RX(&sr, 1);
	CS_HIGH();

	return sr;
}

/* ===========================================================================
 * Function: WriteStatusRegister
 * ===========================================================================
 * @brief
 *  - Executes WRITE STATUS REGISTER (1Fh) to modify configuration registers.
 *
 * @details
 *  - Updates one selected status/configuration register with the given value.
 *  - Requires WriteEnable(06h) before issuing this command.
 *  - Used to enable features such as QE (Quad Enable) or ECC.
 *  - The modification becomes effective after internal write completes
 *    (tFEAT ≈ 200–500 µs).
 *
 *  Command Sequence:
 *    [1Fh] + [SR_Addr] + [Data_In]
 *
 * @param sr_addr : Target status register address (e.g., 0xA0, 0xB0, 0xC0).
 * @param value   : Data byte to be written to the selected register.
 *
 * @note
 *  - Ensure SR[0] (OIP = 0) before issuing command.
 *  - Verify change by reading back with ReadStatusRegister().
 *  - Reference: Winbond W25N02KV Datasheet 8.2.4
 * --------------------------------------------------------------------------- */
void WriteStatusRegister(uint8_t sr_addr, uint8_t value)
{
	uint8_t command[3] = { WRITE_SR, sr_addr, value };

	CS_LOW();
	HAL_SPI_TX(command, 3);
	CS_HIGH();
}
