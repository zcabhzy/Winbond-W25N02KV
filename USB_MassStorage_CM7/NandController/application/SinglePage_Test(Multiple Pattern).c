/*
 *  SinglePage_Test(Multiple Pattern).c
 *
 *  Created on: Oct 14, 2025
 *  Author: Henry
 */

#include "SinglePage_Test(Multiple Pattern).h"

/* ===========================================================================
 * Function : MultiplePattern_Test
 * ===========================================================================
 * @brief
 *   Execute page program / read / verify test with multiple data patterns.
 *
 * @details
 *   - This routine validates data integrity under various bit patterns.
 *   - It performs full block erase, single-page program, and read-back verify.
 *   - The write pattern cycles through 0xAA, 0x55, 0xFF, and 0x00 sequences
 *     to exercise different bit-transition conditions.
 *   - Intended for reliability and controller path validation (not production).
 *
 *   Functional Flow:
 *     1. Issue software reset to clear status.
 *     2. Erase target block and prepare pattern test data.
 *     3. Program one page with random column address (Random Program mode).
 *     4. Read back same page using Random Read command.
 *     5. Verify read data and report mismatch if found.
 *     6. Display SR1–SR3 contents and re-erase block for cleanup.
 *
 * @param  block_num: Logical block index to test.
 *
 * @return None(Results are printed via UART console.)
 *
 * @note
 *   - Page size : 2 KB (main area only).
 *   - Block size: 128 KB = 64 pages.
 *   - Pattern sequence used: 0xAA → 0x55 → 0xFF → 0x00 (repeating).
 *   - SR bits:
 *       SR1[0]=OIP, SR2[2]=E_FAIL, SR3[3]=P_FAIL, SR3[5:4]=ECC[1:0].
 * --------------------------------------------------------------------------- */
void MultiplePattern_Test(uint32_t block_num)
{
	uint8_t readBuffer[PAGE_MAIN_SIZE];
	uint8_t writeBuffer[PAGE_MAIN_SIZE];
	uint32_t base_page = block_num * PAGES_PER_BLOCK;

	printf("=========================================================\r\n");
	printf("============= [MultiplePattern Test Start] ==============\r\n");

	/* -----------------------------------------------------------------------
	 * Step 1. Device reset and status check
	 * -----------------------------------------------------------------------
	 * - [66h + 99h] Software Reset command sequence.
	 * - Clears SR1–SR3 and aborts any ongoing operation.
	 * - Optional: Verify SR1[0]=0 (OIP=0) to confirm device idle state.
	 */
	SoftwareReset_service();

	if (GetSR1() & 0x01)
		printf("[SR1] Busy after Reset\r\n");

	/* -----------------------------------------------------------------------
	 * Step 2. Block erase and pattern preparation
	 * -----------------------------------------------------------------------
	 * - [D8h] Block Erase (128 KB unit).
	 * - Wait until SR1[OIP]=0 or timeout reached.
	 * - Check SR2[E_FAIL] for erase failure.
	 * - Prepare multi-pattern data sequence: 0xAA, 0x55, 0xFF, 0x00 repeated.
	 */
	BlockErase128K_service(block_num, 500);
	PreparePattern(writeBuffer, PAGE_MAIN_SIZE, PATTERN_SEQ_AA55FF00);

	/* -----------------------------------------------------------------------
	 * Step 3. Page program (random column mode)
	 * -----------------------------------------------------------------------
	 * - [06h] Write Enable → [02h] Load Program Data → [10h] Program Execute.
	 * - Program data to first page in block (base_page) with column = 0x0000.
	 * - Check SR3[P_FAIL] bit for any program failure.
	 */
	RandomProgram_Service(base_page, 0x0000, writeBuffer, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 4. Page read (random column mode)
	 * -----------------------------------------------------------------------
	 * - [13h] Page Read to Cache → [03h] Read Data.
	 * - Read back same page at column address 0x0000.
	 * - ECC result indicated by SR3[5:4]:
	 *     00 = OK, 01 = Corrected, 10 = Corrected at threshold, 11 = Uncorrectable.
	 */
	RandomRead_Service(base_page, 0x0000, readBuffer, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 5. Data verification
	 * -----------------------------------------------------------------------
	 * - Compare read data with original write buffer.
	 * - Report first mismatch location for analysis.
	 */
	for (int i = 0; i < PAGE_MAIN_SIZE; i++)
	{
		if (readBuffer[i] != writeBuffer[i])
		{
			printf("[Verify] Mismatch Byte : %d\r\n", i);
			printf("[Verify] Expect : 0x%02X\r\n", writeBuffer[i]);
			printf("[Verify] Get : 0x%02X\r\n", readBuffer[i]);
			break;
		}
	}

	/* -----------------------------------------------------------------------
	 * Step 6. Status register dump / Cleanup
	 * -----------------------------------------------------------------------
	 * - Display SR1–SR3 for post-test inspection.
	 * - Normal condition: E_FAIL=0, P_FAIL=0, WEL=0, OIP=0.
	 * - [D8h] Erase block again to restore clean state for next run.
	 */
	printf("[SR] SR1 = 0x%02X, SR2 = 0x%02X, SR3 = 0x%02X\r\n", GetSR1(),
			GetSR2(), GetSR3());
	printf("[MultiplePattern Test] Block %lu Test Finished\r\n", block_num);

	BlockErase128K_service(block_num, 500);

	printf("============ [MultiplePattern Test Finished] ============\r\n");
	printf("=========================================================\r\n");
}
