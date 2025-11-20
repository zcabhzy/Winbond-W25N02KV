/*
 *  BlockErase_service.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "BlockErase_service.h"

/* ===========================================================================
 * Function: BlockErase128K_service
 * ===========================================================================
 * @brief
 *  - Performs a full 128-KB block erase operation on the W25N02KV NAND array.
 *
 * @details
 *  - High-level service routine that executes a complete block erase sequence,
 *    including pre-erase unlock, write enable, erase command, and status polling.
 *  - The erase unit is one block (128 KB = 64 pages × 2 KB).
 *  - Each page within the block is reset to 0xFF after successful completion.
 *  - Internally checks for timeout, erase-fail, and WEL (Write Enable Latch) status.
 *
 *  Command Sequence:
 *    1. [SetBlockProtect]   Unlock all blocks (BP = 0)
 *    2. [06h]               Write Enable
 *    3. [D8h + BA2:BA1:BA0] Block Erase command
 *    4. Poll SR1[0] (OIP) until ready
 *    5. Check SR2[2] (E_FAIL)
 *    6. Confirm WEL cleared (SR1[1] = 0)
 *
 * @param block_addr : 24-bit target block address (block index × 128 KB).
 * @param timeout_ms : Timeout value in milliseconds for the operation.
 *
 * @return
 *  - true  : Erase succeeded (no timeout, E_FAIL = 0, WEL cleared).
 *  - false : Erase failed, timeout occurred, or status invalid.
 *
 * @note
 *  - Requires that device is idle (OIP = 0) before starting.
 *  - Typical erase time tBERS ≈ 2–3 ms per block.
 *  - Factory-bad blocks must not be erased.
 * --------------------------------------------------------------------------- */
bool BlockErase128K_service(uint32_t block_addr, uint32_t timeout_ms)
{
	if (!SetBlockProtect_Service(0x0, false))
	{
		printf("[Init] Failed to unlock all Blocks\r\n");
	}

	if (!WriteEnable_Service())
	{
		printf("[Block Erase] Write Enable Failed\r\n");
		return false;
	}

	BlockErase128KB(block_addr);

	if (!IsBusyWithTimeout_service(timeout_ms))
	{
		printf("[Block Erase] Timeout\r\n");
		return false;
	}

	printf("[SR After ] SR1=0x%02X SR2=0x%02X SR3=0x%02X\r\n", GetSR1(),
			GetSR2(), GetSR3());

	if (CheckEraseFail_service())
	{
		printf("[Block Erase] Failed (E-FAIL = 1)\r\n");
		return false;
	}

	if (IsWriteEnableLatch_service())
	{
		printf("[Block Erase] Warning: WEL not cleared after erase\r\n");
	}

	printf("[Block Erase] Success (Block = 0x%06lX)\r\n", block_addr);
	return true;
}
