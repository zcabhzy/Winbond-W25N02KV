/*
 *  FactoryInvalidBlockScan_Test.c
 *
 *  Created on: Oct 14, 2025
 *  Author: Henry
 *  Folder: NandController/application
 */

#include "FactoryInvalidBlockScan_Test.h"

/* ===========================================================================
 * Function : ScanInvaliBlocks
 * ===========================================================================
 * @brief
 *   Perform a full scan of factory-marked invalid blocks and print summary.
 *
 * @details
 *   - Executes a status clear and device reset to ensure clean test start.
 *   - Scans all factory-defined invalid blocks (excluding reserved ones).
 *   - Factory bad blocks are identified by spare area marker (Spare[0] != 0xFF).
 *   - ECC must be enabled; blocks [0–7] and [2044–2047] are always valid regions.
 *   - Displays BBT summary and bad block list after scan.
 *
 *   Functional Flow:
 *     1. Issue software reset to clear any previous status.
 *     2. Call BBT_ScanFactoryBlocks() to detect all factory-marked bad blocks.
 *     3. Print scan summary information.
 *     4. Display invalid block list.
 *
 * @param  None
 * @return None
 * --------------------------------------------------------------------------- */
void ScanInvaliBlocks(void)
{
	printf("=========================================================\r\n");
	printf("============ [Scan Invalid Block Test Start] ============\r\n");

	/* -----------------------------------------------------------------------
	 * Step 1. Device reset and status clear
	 * -----------------------------------------------------------------------
	 * - [66h + 99h] Software Reset command sequence.
	 * - Ensures no ongoing operation and SRx registers are reset.
	 */
	SoftwareReset_service();

	/* -----------------------------------------------------------------------
	 * Step 2. Factory bad block scan
	 * -----------------------------------------------------------------------
	 * - Scan valid range: Block 8 to 2043.
	 * - Condition: Spare[0] != 0xFF indicates factory-marked bad block.
	 * - Blocks 0–7 and 2044–2047 are reserved and always valid.
	 */
	BBT_ScanFactoryBlocks();

	/* -----------------------------------------------------------------------
	 * Step 3. Scan result summary
	 * -----------------------------------------------------------------------
	 * - Display total blocks scanned and number of bad blocks found.
	 */
	BBT_PrintSummary();

	/* -----------------------------------------------------------------------
	 * Step 4. Bad block list output
	 * -----------------------------------------------------------------------
	 * - Display each bad block index and marker value.
	 */
	BBT_ShowBadBlock();

	printf("=========== [Scan Invalid Block Test Finished] ==========\r\n");
	printf("=========================================================\r\n");
}

/* ===========================================================================
 * Function : ChoseValidBlock
 * ===========================================================================
 * @brief
 *   Find the first valid (good) block after scanning and run basic test.
 *
 * @details
 *   - Performs a complete factory invalid block scan first.
 *   - Retrieves the first valid block index from BBT.
 *   - Executes unit test routine on the selected valid block.
 *   - Prints "No valid block" if no usable block is found.
 *
 *   Functional Flow:
 *     1. Execute ScanInvaliBlocks() to update bad block table.
 *     2. Find first valid block using FindFirstValidBlock().
 *     3. Run Standard_UnitTest() on the valid block for verification.
 *
 * @param  None
 * @return None
 * --------------------------------------------------------------------------- */
void ChoseValidBlock(void)
{
	/* -----------------------------------------------------------------------
	 * Step 1. Perform factory bad block scan.
	 * -----------------------------------------------------------------------
	 * - Ensure BBT is updated before selecting valid block.
	 */
	ScanInvaliBlocks();

	/* -----------------------------------------------------------------------
	 * Step 2. Retrieve first valid block index.
	 * -----------------------------------------------------------------------
	 * - Returns negative value if no valid block available.
	 */
	int validBlock = FindFirstValidBlock();
	printf("\r\n");

	/* -----------------------------------------------------------------------
	 * Step 3. Execute unit test on valid block.
	 * -----------------------------------------------------------------------
	 * - Use Standard_UnitTest() to verify basic program/erase/read sequence.
	 */
	if (validBlock >= 0)
	{
		printf("First Valid Block : %d\r\n", validBlock);
		Standard_UnitTest(validBlock);
	}
	else
	{
		printf("No valid block available !\r\n");
	}
}

