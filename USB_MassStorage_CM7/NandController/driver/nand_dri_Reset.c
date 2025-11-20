/*
 *  nand_dri_Reset.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/driver
 */

#include "nand_dri_Reset.h"

/* ===========================================================================
 * Function: Reset
 * ===========================================================================
 * @brief
 *  - Executes SOFTWARE RESET (99h) to reset internal state machine.
 *
 * @details
 *  - Clears status registers and aborts any ongoing program/erase operation.
 *  - Must be preceded by EnableReset(66h) to unlock the reset sequence.
 *  - Typical reset recovery time: tRST ≈ 5–10 ms.
 *  - After execution, all volatile register values return to default.
 *
 * Command Sequence:
 *  - [66h] -> [99h]
 *
 * @note
 *  - Poll SR[0] (OIP = 0) before issuing new commands.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.1
 * --------------------------------------------------------------------------- */
void Reset(void)
{
	uint8_t command = RESET;
	CS_LOW();
	HAL_SPI_TX(&command, 1);
	CS_HIGH();
}

/* ===========================================================================
 * Function: EnableReset
 * ===========================================================================
 * @brief
 *  - Issues ENABLE RESET (66h) command to allow subsequent device reset.
 *
 * @details
 *  - Acts as a safety unlock before RESET (99h).
 *  - Prevents accidental reset during normal operation.
 *
 * @note
 *  - Must be followed by Reset(99h) within tWRESET interval (typically < 1 ms).
 *  - Reference: Winbond W25N02KV Datasheet 8.2.1
 * --------------------------------------------------------------------------- */
void EnableReset(void)
{
	uint8_t command = ENABLE_RESET;
	CS_LOW();
	HAL_SPI_TX(&command, 1);
	CS_HIGH();
}

/* ===========================================================================
 * Function: DeviceReset
 * ===========================================================================
 * @brief
 *  - Performs full device reset sequence: ENABLE RESET (66h) + RESET (99h).
 *
 * @details
 *  - Ensure proper internal controller re-initialization.
 *  - Clears all volatile settings and resets ECC, status, and state machine.
 *  - Required after power-up anomalies or timeout error recovery.
 *
 * Flow:
 *  -> EnableReset(66h)
 *  -> Reset(99h)
 *
 * @note
 *  - Wait until SR[0] (OIP = 0) before accessing the device again.
 *  - Reference: Winbond W25N02KV Datasheet 8.2.1
 * --------------------------------------------------------------------------- */
void DeviceReset(void)
{
	uint8_t command = DEVICE_RESET;
	CS_LOW();
	HAL_SPI_TX(&command, 1);
	CS_HIGH();
}
