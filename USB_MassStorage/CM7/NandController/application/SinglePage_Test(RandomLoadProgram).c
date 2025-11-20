/*
 *  SinglePage_Test(RandomLoadProgram).c
 *
 *  Created on: Oct 14, 2025
 *  Author: Henry
 */

#include "SinglePage_Test(RandomLoadProgram).h"

/* ===========================================================================
 * Function : Random_UnitTest
 * ===========================================================================
 * @brief
 *   Perform a single-page random column address program/read verification test.
 *
 * @details
 *   - This routine validates the controller’s random column addressing path.
 *   - Uses [84h] Random Load command instead of sequential [02h].
 *   - Confirms correct column offset handling in both program and read paths.
 *   - Verifies data integrity by comparing read-back data with expected pattern.
 *
 *   Functional Flow:
 *     1. Software reset and status clear.
 *     2. Erase target block (128 KB) before programming.
 *     3. Load pattern (0xAA) and program one page using [84h] random load.
 *     4. Read back the same page using Random Read command.
 *     5. Verify data consistency and report mismatch.
 *     6. Dump SR1–SR3 for status verification and clean up block.
 *
 * @param  block_num
 *   Logical block index to be tested.
 *
 * @return
 *   None. Test progress and results are printed to console.
 *
 * @note
 *   - Page size : 2048 bytes (main area).
 *   - Block size: 64 pages (128 KB).
 *   - Related NAND commands:
 *       [66h + 99h] Software Reset
 *       [D8h] Block Erase
 *       [06h] Write Enable
 *       [84h] Random Data Load (Program)
 *       [10h] Program Execute
 *       [13h] Page Read to Cache
 *       [03h] Read Data (Serial Output)
 *   - Status Register bits:
 *       SR1[0] = OIP, SR2[2] = E_FAIL, SR3[3] = P_FAIL, SR3[5:4] = ECC bits
 * --------------------------------------------------------------------------- */
void Random_UnitTest(uint32_t block_num)
{
	uint8_t readBuffer[PAGE_MAIN_SIZE];
	uint8_t writeBuffer[PAGE_MAIN_SIZE];
	uint32_t base_page = block_num * PAGES_PER_BLOCK;

	printf("=========================================================\r\n");
	printf("=============== [Random UnitTest Start] =================\r\n");

	/* -----------------------------------------------------------------------
	 * Step 1. Device reset and status clear
	 * -----------------------------------------------------------------------
	 * - [66h + 99h] Software Reset command.
	 * - Ensures no pending operation and clears SR1–SR3.
	 */
	SoftwareReset_service();

	/* -----------------------------------------------------------------------
	 * Step 2. Block erase and data preparation
	 * -----------------------------------------------------------------------
	 * - [D8h] Block Erase command (128 KB).
	 * - Wait until SR1[OIP]=0 or timeout (500 ms).
	 * - Verify SR2[E_FAIL]=0 after completion.
	 * - Prepare 0xAA pattern for program phase.
	 */
	BlockErase128K_service(block_num, 500);
	PreparePattern(writeBuffer, PAGE_MAIN_SIZE, PATTERN_AA);

	/* -----------------------------------------------------------------------
	 * Step 3. Random column program
	 * -----------------------------------------------------------------------
	 * - [06h] Write Enable → [84h] Random Load → [10h] Program Execute.
	 * - Uses column address 0x0000 (can be adjusted for partial-page testing).
	 * - Check SR3[P_FAIL] for program failure.
	 */
	RandomProgram_Service(base_page, 0x0000, writeBuffer, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 4. Random column read
	 * -----------------------------------------------------------------------
	 * - [13h] Page Read to Cache → [03h] Read Data.
	 * - Column address = 0x0000 (same as program address).
	 * - ECC result reflected in SR3[5:4]:
	 *     00 = OK, 01 = Corrected, 10 = Corrected at threshold, 11 = Uncorrectable.
	 */
	RandomRead_Service(base_page, 0x0000, readBuffer, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 5. Data verification
	 * -----------------------------------------------------------------------
	 * - Compare read buffer content against 0xAA pattern.
	 * - Report first mismatched byte index and values.
	 */
	for (int i = 0; i < PAGE_MAIN_SIZE; i++)
	{
		if (readBuffer[i] != 0xAA)
		{
			printf("[Verify] Mismatch Byte : %d\r\n", i);
			printf("[Verify] Expect : 0x%02X\r\n", writeBuffer[i]);
			printf("[Verify] Get : 0x%02X\r\n", readBuffer[i]);
			break;
		}
	}

	/* -----------------------------------------------------------------------
	 * Step 6. Status register dump
	 * -----------------------------------------------------------------------
	 * - Display SR1–SR3 for post-test validation.
	 * - Normal case: OIP=0, WEL=0, E_FAIL=0, P_FAIL=0.
	 */
	printf("[Status Register] SR1:0x%02X SR2:0x%02X SR3:0x%02X\r\n", GetSR1(),
			GetSR2(), GetSR3());
	printf("[Random UnitTest] Block %lu Test Finished\r\n", block_num);

	/* -----------------------------------------------------------------------
	 * Step 7. Cleanup
	 * -----------------------------------------------------------------------
	 * - [D8h] Erase the block again to restore clean state.
	 * - Ensures block can be reused for subsequent tests.
	 */
	BlockErase128K_service(block_num, 500);

	printf("============== [Random UnitTest Finished] ===============\r\n");
	printf("=========================================================\r\n");
}
