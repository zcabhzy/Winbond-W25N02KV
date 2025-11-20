/*
 *  EnduranceTest.c
 *
 *  Created on: Oct 5, 2025
 *  Author: Henry
 *  Folder: NandController/application
 */

#include "Endurance_Test.h"

/* ===========================================================================
 * Function : EnduranceTest_Run
 * ===========================================================================
 * @brief
 *   Perform NAND block endurance test by repeated erase / program / read cycles.
 *
 * @details
 *   - This routine evaluates the P/E (Program-Erase) endurance of a single block.
 *   - Each cycle performs a full block erase followed by page program + read verify.
 *   - Test stops immediately if program, erase, or ECC uncorrectable error occurs.
 *   - Used for reliability evaluation, not for production runtime.
 *
 *   Functional Flow:
 *     1. Issue software reset and prepare write pattern.
 *     2. Loop for MAX_PE_CYCLE times:
 *         (a) Erase target block, verify erase status.
 *         (b) Program all pages with predefined data pattern.
 *         (c) Read back programmed data and perform byte-by-byte verify.
 *         (d) Check ECC status and NAND SR1–SR3 for P_FAIL/E_FAIL.
 *         (e) Stop test if any failure condition detected.
 *
 * @param block: Logical block index to be tested.
 *
 * @return None(Test result is reported via console log.)
 *
 * @note
 *   - Each block = 128 KB (64 pages × 2 KB main).
 *   - Typical P/E endurance = 60K cycles depending on NAND grade.
 *   - This function is for engineering validation only.
 * --------------------------------------------------------------------------- */
void EnduranceTest_Run(uint32_t block)
{
	bool verifyFail = false;
	uint8_t readBuffer[PAGE_MAIN_SIZE];
	uint8_t writeBuffer[PAGE_MAIN_SIZE];
	uint32_t base_page = block * PAGES_PER_BLOCK;

	printf("==========================================================\r\n");
	printf("================ [Endurance Test Started] ================\r\n");
	printf("[Endurance Test] Block: %lu \r\n", block);
	printf("[Endurance Test] Cycle: %d  \r\n", MAX_PE_CYCLE);

	/// Step 1:
	/// [66h + 99h]: Clear status register and terminate any ongoing operations
	/// Prepare test data pattern: 0xAA
	SoftwareReset_service();
	PreparePattern(writeBuffer, PAGE_MAIN_SIZE, PATTERN_AA);

	for (uint32_t cycle = 0; cycle < MAX_PE_CYCLE; cycle++)
	{
		/// Step 2:
		/// [D8h] Erase Block -> Check Status Register (S2: E_Fail)
		if (!BlockErase128K_service(block, 500))
			printf("[Endurance Test] Cycle %lu Erase Failed\r\n", cycle);

		for (uint32_t page = 0; page < PAGES_PER_BLOCK; page++)
		{
			uint32_t page_addr = base_page + page;

			/// Step 3:
			/// [06h] Write Enable -> [02h] Load Program Data -> [10h] Program Execute
			/// Check Status Register (S3: P_Fail), return summary when test fail
			if (!StandardProgram_Service(page_addr, writeBuffer,
			PAGE_MAIN_SIZE))
			{
				printf("[Endurance] Cycle %lu\r\n", cycle);
				printf("[Endurance] Page %lu Program Fail\r\n", page);
				verifyFail = true;
				break;
			}

			/// Step 4:
			/// [13h] Page Data Read -> [03h] Read Data
			/// Check ECC Status (00|01|10|11), return summary when test fail
			if (!StandardRead_Service(page_addr, 0x0000, readBuffer,
			PAGE_MAIN_SIZE))
			{
				printf("[Endurance] Cycle %lu\r\n", cycle);
				printf("[Endurance] Page %lu Read Failed\r\n", page);
				verifyFail = true;
				break;
			}

			for (int i = 0; i < PAGE_MAIN_SIZE; i++)
			{
				if (readBuffer[i] != writeBuffer[i])
				{
					printf("[Endurance] Verify Mismatch at Byte %d \r\n", i);
					printf("[Endurance] Exp : 0x%02X\r\n", writeBuffer[i]);
					printf("[Endurance] Got : 0x%02X\r\n", readBuffer[i]);
					verifyFail = true;
					break;
				}
			}
			if (verifyFail)
				break;
		}
		if (verifyFail)
			break;

		/// Step 5:
		/// Check ECC and Status Registers (SR1 - SR3)
		/// If P_Fail / E_Fail / ECC Status == ECC_UNCORRECTABLE, End Test
		ECC_Status_t ecc = GetECCStatus_service();
		uint8_t sr1 = GetSR1();
		uint8_t sr2 = GetSR2();
		uint8_t sr3 = GetSR3();

		printf("[Cycle %lu] SR1=0x%02X SR2=0x%02X SR3=0x%02X ECC=%d\r\n", cycle,
				sr1, sr2, sr3, ecc);

		if (CheckEraseFail_service() || CheckProgramFail_service())
		{
			printf("[Cycle %lu] EFAIL/PFAIL Detected\r\n", cycle);
			break;
		}

		if (ecc == ECC_UNCORRECTABLE)
		{
			printf("[Cycle %lu] ECC Uncorrectable -> End Test\r\n", cycle);
			break;
		}

		/// Step 6:
		/// Display progress
		if ((cycle + 1) % 100 == 0)
		{
			printf("[Progress] %lu cycles completed\r\n", cycle + 1);
		}
	}

	printf("=============== [Endurance Test Finished] ===============\r\n");
	printf("=========================================================\r\n");
}
