/*
 *  Reset_service.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "Reset_service.h"

/* ===========================================================================
 * Function: DeviceReset_service
 * ===========================================================================
 * @brief
 *  - Performs a full hardware reset of the NAND device (FFh).
 *
 * @details
 *  - Issues the DEVICE RESET (FFh) command to reinitialize all internal logic,
 *    including cache registers, ECC engine, and status registers.
 *  - Used after power-up, bus fault, or unresponsive state recovery.
 *  - After reset, the device requires a typical tRST delay (≈5–10 ms)
 *    before accepting new commands.
 *  - Recommended at system startup to ensure a known-good state.
 *
 *  Command Flow:
 *    1. [FFh] Device Reset
 *    2. Wait tRST (host should poll R/B# or delay ~10 ms)
 *
 * @return
 *  - true  : Command issued successfully.
 *  - false : Reserved (no hardware feedback available in current layer).
 * --------------------------------------------------------------------------- */
bool DeviceReset_service(void)
{
	DeviceReset();

	printf("[Device Reset] Success\r\n");
	return true;
}

/* ===========================================================================
 * Function: SoftwareReset_service
 * ===========================================================================
 * @brief
 *  - Performs a software-triggered reset sequence (66h + 99h).
 *
 * @details
 *  - Executes the Enable Reset (66h) followed by Reset (99h) command sequence.
 *  - Used when the NAND device enters a busy or abnormal state but power cycling
 *    is not desirable.
 *  - This reset clears operation-in-progress (OIP) status and internal caches
 *    without erasing data.
 *  - After reset, SR1, SR2, and SR3 return to default values.
 *  - Recommended before reinitializing SPI interface or ECC mode.
 *
 *  Command Flow:
 *    1. [66h] Enable Reset
 *    2. [99h] Software Reset
 *    3. Wait tRST (~5–10 ms)
 *
 * @return
 *  - true  : Reset command sequence completed successfully.
 *  - false : Reserved (for future error handling).
 * --------------------------------------------------------------------------- */
bool SoftwareReset_service(void)
{
	EnableReset();
	Reset();

	printf("[Software Reset] Success\r\n");
	return true;
}
