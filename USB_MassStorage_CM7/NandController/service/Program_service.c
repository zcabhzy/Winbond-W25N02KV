/*
 *  Program_service.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "Program_service.h"

/* ===========================================================================
 * Function: StandardProgram_Service
 * ===========================================================================
 * @brief
 *  - Performs a full-page program sequence (02h + 10h) on the W25N02KV NAND device.
 *
 * @details
 *  - High-level service API that combines the standard program command sequence.
 *  - Programs one complete page starting at column address 0x0000.
 *  - Used for normal main-area data programming (2 KB per page).
 *  - Internally handles write-enable, data load, program execution, and status check.
 *
 *  Command Flow:
 *    1. [06h] Write Enable (WEL = 1)
 *    2. [02h + Column Addr] Load Program Data into cache buffer
 *    3. [10h + Page Addr]   Program Execute (commit data to array)
 *    4. Poll SR1[0] (OIP) until ready
 *    5. Check SR3[3] (P_FAIL) for program failure
 *
 * @param page_addr : Target page address to be programmed.
 * @param buf       : [in] Pointer to data buffer containing data to be written.
 * @param len       : Length of data in bytes to program.
 *
 * @return
 *  - true  : Program operation succeeded (no timeout, P_FAIL = 0).
 *  - false : Program failed or timeout occurred.
 *
 * @note
 *  - Requires Write Enable (WEL = 1) before issuing 02h.
 *  - Typical program time (tPROG) ≈ 200–600 µs.
 *  - For partial-page update, use RandomProgram_Service().
 *  - Reference: Winbond W25N02KV Datasheet §7.3.5
 * --------------------------------------------------------------------------- */
bool StandardProgram_Service(uint32_t page_addr, const uint8_t *buf, uint16_t len)
{
	/// Step 1: Write Enable
	if (!WriteEnable_Service())
		return false;

	/// Step 2: 將資料從 Column=0x0000 開始載入 Page Buffer
	/// Step 3: 執行 Program Execute，把 Buffer 資料寫進 NAND Page
	LoadProgramData(0x0000, buf, len);
	ProgramExecute(page_addr);

	/// Step 4: 等待程式完成 (Polling SR3 OIP bit)
	if (!IsBusyWithTimeout_service(100))
		return false;

	/// Step 5: 檢查 Program Fail (P_Fail=1 代表失敗)
	if (CheckProgramFail_service() == true)
	{
		printf("[Standard Program] Failed (P_Fail = 1)\r\n");
		return false;
	}
	else
	{
		printf("[Standard Program] Success (P_Fail = 0)\r\n");
		return true;
	}
}

/* ===========================================================================
 * Function: RandomProgram_Service
 * ===========================================================================
 * @brief
 *  - Performs a random page program sequence (84h + 10h) to update partial data.
 *
 * @details
 *  - Allows programming data to a specific column address within a page buffer.
 *  - Useful for updating spare area, OOB (Out-Of-Band), or partial main data.
 *  - Multiple Random Load (84h) commands can be issued before a single
 *    Program Execute (10h) to program combined data in one operation.
 *
 *  Command Flow:
 *    1. [06h] Write Enable (WEL = 1)
 *    2. [84h + Column Addr] Random Load Program Data
 *    3. [10h + Page Addr]   Program Execute
 *    4. Poll SR1[0] (OIP) until ready
 *    5. Check SR3[3] (P_FAIL) for program failure
 *
 * @param page_addr : Target page address to be programmed.
 * @param col_addr  : Column start address within the page buffer.
 * @param buf       : [in] Pointer to data buffer to be written.
 * @param len       : Number of bytes to program.
 *
 * @return
 *  - true  : Random program succeeded (no timeout, P_FAIL = 0).
 *  - false : Program failed or timeout occurred.
 *
 * @note
 *  - Random Program supports multiple data-in before final execute.
 *  - Requires WEL = 1 before first 84h command.
 *  - Used primarily for metadata/spare-area updates.
 *  - Reference: Winbond W25N02KV Datasheet §7.3.6
 * --------------------------------------------------------------------------- */
bool RandomProgram_Service(uint32_t page_addr, uint16_t col_addr, const uint8_t *buf, uint16_t len)
{
	/// Step 1: Write Enable
	if (!WriteEnable_Service())
		return false;

	/// Step 2: 載入資料到 Page Buffer (可多次呼叫 0x84，更新不同 col_addr)
	/// Step 3: 執行 Program Execute，把整個 Page Buffer 寫進 NAND
	RandomLoadProgramData(col_addr, buf, len);
	ProgramExecute(page_addr);

	/// Step 4: 等待程式完成 (Polling SR3 OIP bit)
	IsBusyWithTimeout_service(100);

	/// Step 5: 檢查 Program Fail (P_Fail = 1)
	if (CheckProgramFail_service())
	{
		printf("[Random Program] Failed (P_Fail = 1)\r\n");
		return false;
	}
	else
	{
		printf("[Random Program] Success (P_Fail = 0)\r\n");
		return true;
	}
}
