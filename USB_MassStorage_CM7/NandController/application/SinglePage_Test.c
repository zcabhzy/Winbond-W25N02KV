/*
 *  SinglePage_Test.c
 *
 *  Created on: Oct 14, 2025
 *  Author: Henry
 */

#include "SinglePage_Test.h"

/* ===========================================================================
 * Function : Standard_UnitTest
 * ===========================================================================
 * @brief
 *   Perform a single-page program and read-back verification test on one block.
 *
 * @details
 *   - This routine executes a complete program-read-verify cycle to confirm
 *     correct command sequence and ECC operation.
 *   - It erases the target block, programs one page with a known pattern,
 *     reads it back, and compares data byte-by-byte.
 *   - Intended for basic functional test and controller/NAND interface check.
 *
 *   Functional Flow:
 *     1. Reset device and clear all status registers.
 *     2. Erase one target block (128 KB).
 *     3. Prepare 0xAA data pattern and program one page.
 *     4. Read back programmed page and verify data integrity.
 *     5. Dump SR1–SR3 to confirm correct status after operation.
 *     6. Re-erase block to restore clean state.
 *
 * @param  block_num
 *   Logical block index to test.
 *
 * @return
 *   None. Test result is printed to console.
 *
 * @note
 *   - Page size: 2048 bytes (main area only).
 *   - Block size: 64 pages (128 KB total).
 *   - SR bits reference:
 *       SR1[0] = OIP, SR2[2] = E_FAIL, SR3[3] = P_FAIL, SR3[5:4] = ECC status.
 * --------------------------------------------------------------------------- */
void Standard_UnitTest(uint32_t block_num)
{
	uint8_t write_buf[PAGE_MAIN_SIZE];
	uint8_t read_buf[PAGE_MAIN_SIZE];
	uint32_t base_page = block_num * PAGES_PER_BLOCK;

	printf("=========================================================\r\n");
	printf("=============== [Standard UnitTest Start] ===============\r\n");

	/* -----------------------------------------------------------------------
	 * Step 1. Device reset and status clear
	 * -----------------------------------------------------------------------
	 * - [66h + 99h] Software Reset command sequence.
	 * - Ensures no ongoing operation and clears SR1 ~ SR3.
	 */
	SoftwareReset_service();

	/* -----------------------------------------------------------------------
	 * Step 2. Block erase
	 * -----------------------------------------------------------------------
	 * - [D8h + BA2:BA1:BA0] Block Erase command.
	 * - Wait until SR1[OIP] = 0 or timeout reached.
	 * - Check SR2[E_FAIL] after completion.
	 * - Prepare test data pattern 0xAA for program phase.
	 */
	BlockErase128K_service(block_num, 500);
	PreparePattern(write_buf, PAGE_MAIN_SIZE, PATTERN_AA);

	/* -----------------------------------------------------------------------
	 * Step 3. Page program
	 * -----------------------------------------------------------------------
	 * - [06h] Write Enable → [02h] Load Program Data → [10h] Program Execute.
	 * - Program first page in the block (base_page).
	 * - Check SR3[P_FAIL] bit for any program error.
	 */
	StandardProgram_Service(base_page, write_buf, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 4. Page read and ECC check
	 * -----------------------------------------------------------------------
	 * - [13h] Page Data Read → [03h] Read Data command sequence.
	 * - Transfer data to read buffer.
	 * - ECC status indicated by SR3[5:4]:
	 *      00 = Success
	 *      01 = Corrected
	 *      10 = Corrected at threshold
	 *      11 = Uncorrectable
	 */
	StandardRead_Service(base_page, 0x0000, read_buf, PAGE_MAIN_SIZE);

	/* -----------------------------------------------------------------------
	 * Step 5. Data verification
	 * -----------------------------------------------------------------------
	 * - Compare each byte of read data against 0xAA pattern.
	 * - Stop verification if any mismatch is found.
	 */
	for (int i = 0; i < PAGE_MAIN_SIZE; i++)
	{
		if (read_buf[i] != 0xAA)
		{
			printf("[Verify] Mismatch Byte : %d\r\n", i);
			printf("[Verify] Expect : 0x%02X\r\n", 0xAA);
			printf("[Verify] Get : 0x%02X\r\n", read_buf[i]);
			return;
		}
	}

	/* -----------------------------------------------------------------------
	 * Step 6. Status register check
	 * -----------------------------------------------------------------------
	 * - Dump SR1–SR3 values for post-operation analysis.
	 * - SR1[1]=WEL should be cleared after program completes.
	 * - SR2[E_FAIL] or SR3[P_FAIL] should remain 0 if successful.
	 */
	printf("[SR] SR1 = 0x%02X, SR2 = 0x%02X, SR3 = 0x%02X\r\n", GetSR1(),
			GetSR2(), GetSR3());
	printf("[UnitTest] Block %lu Test Finished\r\n", block_num);

	/* -----------------------------------------------------------------------
	 * Step 7. Restore block to clean state
	 * -----------------------------------------------------------------------
	 * - [D8h] Block Erase again to remove programmed data.
	 * - Ensures block can be reused for next test iteration.
	 */
	BlockErase128K_service(block_num, 500);

	printf("============== [Standard UnitTest Finished] =============\r\n");
	printf("=========================================================\r\n");
}
