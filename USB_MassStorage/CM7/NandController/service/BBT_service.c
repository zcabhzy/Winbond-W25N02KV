/*
 *  BBT_service.c
 *
 *  Created on: Sep 24, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "BBT_service.h"

bool BBT_Table[TOTAL_BLOCKS]; // Invalid Block Table

extern bool StandardRead_Service(uint32_t page_addr, uint16_t col_addr, uint8_t *buf, uint16_t len);
extern void LoadProgramData(uint16_t col_addr, const uint8_t *buf, uint16_t len);
extern void ProgramExecute(uint32_t page_addr);
extern bool IsBusyWithTimeout_service(uint32_t timeout_ms);
extern bool CheckProgramFail_service(void);

/* ===========================================================================
 * Function: read_marker_bytes_page0
 * ===========================================================================
 * @brief
 *  - Reads the factory bad block marker bytes from page0 of a target block.
 *
 * @details
 *  - Reads one byte from main area [main0] and one from spare area [spare0]
 *    of the first page (page0) in the specified block.
 *  - These bytes are checked to identify factory-marked bad blocks.
 *  - A good block should have both bytes = 0xFF.
 *
 *  Flow:
 *    1. Read main[0] (first byte of main area).
 *    2. Read spare[0] (first byte of spare area).
 *
 * @param block : Target block index to read marker from.
 * @param m0    : Pointer to buffer for main[0] byte.
 * @param s0    : Pointer to buffer for spare[0] byte.
 *
 * @return
 *  - true  : Successfully read both marker bytes.
 *  - false : Read operation failed.
 *
 * @note
 *  - Should only be used during factory BBT scan.
 *  - Reference: Winbond W25N02KV Datasheet §8.1.1
 * --------------------------------------------------------------------------- */
static bool read_marker_bytes_page0(uint32_t block, uint8_t *m0, uint8_t *s0)
{
	uint32_t page0 = PAGE_ADDR(block, 0);

	if (!StandardRead_Service(page0, 0, m0, 1))
		return false;

	if (!StandardRead_Service(page0, PAGE_MAIN_SIZE, s0, 1))
		return false;

	return true;
}

/* ===========================================================================
 * Function: is_factory_bad
 * ===========================================================================
 * @brief
 *  - Determines if a block is factory-marked as bad.
 *
 * @details
 *  - Calls read_marker_bytes_page0() to obtain main0/spare0 bytes.
 *  - A block is considered bad if either marker byte != 0xFF.
 *  - Initializes both bytes to 0xFF before reading to ensure clean state.
 *
 *  Evaluation Logic:
 *    if (main0 != 0xFF) || (spare0 != 0xFF) → Factory bad block
 *
 * @param block   : Target block index to check.
 * @param main0   : Pointer to byte storing main[0] value.
 * @param spare0  : Pointer to byte storing spare[0] value.
 *
 * @return
 *  - true  : Factory bad block detected.
 *  - false : Block is good or unreadable.
 *
 * @note
 *  - Used only during initial BBT scan.
 *  - Reference: Winbond W25N02KV Datasheet §8.1.1
 * --------------------------------------------------------------------------- */
static bool is_factory_bad(uint32_t block, uint8_t *main0, uint8_t *spare0)
{
	*main0 = 0xFF;
	*spare0 = 0xFF;

	if (!read_marker_bytes_page0(block, main0, spare0))
		return false;

	return (*main0 != 0xFFu) || (*spare0 != 0xFFu);
}

/* ===========================================================================
 * Function: write_bad_marker_page0_spare0
 * ===========================================================================
 * @brief
 *  - Programs a bad block marker (0x00) into spare[0] of page0.
 *
 * @details
 *  - Used when marking a runtime bad block detected during operation.
 *  - Writes one byte (0x00) to spare[0] of the first page, then executes
 *    PROGRAM EXECUTE (10h) to permanently mark the block as bad.
 *  - Waits for operation completion and checks for program failure.
 *
 *  Flow:
 *    1. LoadProgramData(spare0, 0x00)
 *    2. ProgramExecute(page0)
 *    3. Poll busy flag or timeout
 *    4. Verify program result via SR3[3] (P_FAIL)
 *
 * @param block : Target block index to mark as bad.
 *
 * @note
 *  - Runtime marker = 0x00 written to spare[0] of page0.
 *  - Should only be called when block fails erase/program validation.
 *  - Reference: Winbond W25N02KV Datasheet §8.1.2
 * --------------------------------------------------------------------------- */
static void write_bad_marker_page0_spare0(uint32_t block)
{
	uint32_t page0 = PAGE_ADDR(block, 0);
	uint8_t mark = 0x00;

	LoadProgramData(PAGE_MAIN_SIZE, &mark, 1);

	ProgramExecute(page0);

	if (!IsBusyWithTimeout_service(100))
	{
		printf("[Invalid Table] Timeout while marking bad block\r\n");
		return;
	}

	if (CheckProgramFail_service())
	{
		printf("[Invalid Table] Failed to program bad block marker\r\n");
	}
	else
	{
		printf("[Invalid Table] Permanent marker written (Block:%lu)\r\n",
				(unsigned long) block);
	}
}

/* ===========================================================================
 * Function: BBT_ScanFactoryBlocks
 * ===========================================================================
 * @brief
 *  - Scans all blocks and builds the Bad Block Table (BBT) based on factory markers.
 *
 * @details
 *  - Reads the first page (page0) of each block to check the factory-programmed
 *    bad block marker in both main area [byte0] and spare area [spare0].
 *  - A block is considered bad if either byte is not 0xFF.
 *  - Factory information regions (reserved system blocks) are skipped from scan.
 *  - Updates BBT_Table[] with "true" for bad blocks and "false" for valid ones.
 *
 *  Command Flow:
 *    1. Read main[0] from page0.
 *    2. Read spare[0] from page0.
 *    3. If either != 0xFF → mark as bad.
 *
 *  @note
 *  - Factory bad blocks must never be used for program/erase operations.
 *  - Reference: Winbond W25N02KV Datasheet §8.1.1
 * --------------------------------------------------------------------------- */
void BBT_ScanFactoryBlocks(void)
{
	uint8_t main0, spare0;

	for (uint32_t blk = 0; blk < TOTAL_BLOCKS; blk++)
	{
		if (blk < FACTORY_INFO_BLOCK_END || blk > FACTORY_INFO_BLOCK2_START)
		{
			BBT_Table[blk] = false;
			continue;
		}

		if (is_factory_bad(blk, &main0, &spare0))
		{
			BBT_Table[blk] = true;
			printf("[Invalid Block] = %lu (m:0x%02X s:0x%02X)\r\n",
					(unsigned long) blk, main0, spare0);
		}
		else
		{
			BBT_Table[blk] = false;
			printf("[Valid Block] = %lu (m:0x%02X s:0x%02X)\r\n",
					(unsigned long) blk, main0, spare0);
		}
	}
}

/* ===========================================================================
 * Function: BBT_MarkRuntimeBad
 * ===========================================================================
 * @brief
 *  - Marks a block as bad during runtime operation.
 *
 * @details
 *  - Triggered when a block encounters erase or program failure.
 *  - Updates BBT_Table[] entry and permanently writes a bad marker
 *    into the spare area of page0.
 *  - This ensures the block is skipped in future allocation.
 *
 *  Flow:
 *    1. Set BBT_Table[block] = true.
 *    2. Write 0x00 marker into spare0 of page0.
 *    3. ProgramExecute() to commit the marker.
 *
 * @param block : Target block index to mark as runtime bad.
 *
 * @note
 *  - Verify OIP = 0 before marker programming.
 *  - Runtime bad marking must be atomic to maintain data integrity.
 * --------------------------------------------------------------------------- */
void BBT_MarkRuntimeBad(uint32_t block)
{
	if (block >= TOTAL_BLOCKS)
		return;

	if (BBT_Table[block])
		return;

	BBT_Table[block] = true;
	printf("[Invalid Table] Runtime Bad Block = %lu\r\n",
			(unsigned long) block);
	write_bad_marker_page0_spare0(block);
}

/* ===========================================================================
 * Function: BBT_IsBad
 * ===========================================================================
 * @brief
 *  - Checks if a specific block is marked as bad.
 *
 * @details
 *  - Performs an instant lookup in the in-memory BBT_Table[].
 *  - Used before any read, program, or erase command to skip bad blocks.
 *
 * @param block : Target block index.
 *
 * @return
 *  - true  : The block is marked as bad.
 *  - false : The block is valid and usable.
 * --------------------------------------------------------------------------- */
bool BBT_IsBad(uint32_t block)
{
	if (block >= TOTAL_BLOCKS)
		return true;

	return BBT_Table[block];
}

/* ===========================================================================
 * Function: BBT_PrintSummary
 * ===========================================================================
 * @brief
 *  - Prints total, bad, and good block counts to the console.
 *
 * @details
 *  - Iterates over BBT_Table[] to compute and display statistics.
 *  - Provides a simple diagnostic summary of NAND health.
 *
 * @note
 *  - Intended for debug and validation in production test stage.
 * --------------------------------------------------------------------------- */
void BBT_PrintSummary(void)
{
	uint32_t bad = 0;

	for (uint32_t i = 0; i < TOTAL_BLOCKS; i++)
	{
		if (BBT_Table[i])
			bad++;
	}

	printf("[Invalid Table] Total = %d, Bad = %lu, Good = %lu\r\n",
	TOTAL_BLOCKS, (unsigned long) bad, (unsigned long) (TOTAL_BLOCKS - bad));
}

/* ===========================================================================
 * Function: BBT_ShowBadBlock
 * ===========================================================================
 * @brief
 *  - Displays indices of all bad blocks stored in BBT_Table[].
 *
 * @details
 *  - Iterates through the table and prints block indices for diagnostic purposes.
 *
 * @note
 *  - Used in debug or production log to visualize bad block distribution.
 * --------------------------------------------------------------------------- */
void BBT_ShowBadBlock(void)
{
	for (uint32_t blk = 0; blk <= TOTAL_BLOCKS; blk++)
	{
		if (BBT_Table[blk])
		{
			printf("[Invalid Table] Invalid Block = %lu\r\n",
					(unsigned long) blk);
		}
	}
}

/* ===========================================================================
 * Function: BBT_GetBadBlocks
 * ===========================================================================
 * @brief
 *  - Collects all bad block indices into a user-provided array.
 *
 * @details
 *  - Scans the BBT_Table[] and copies block numbers into list[].
 *  - Returns the number of bad blocks detected.
 *  - Used by upper-layer FTL or wear-leveling manager.
 *
 * @param list   : [out] Pointer to array for storing block indices.
 * @param maxlen : Maximum number of entries that can be stored in list[].
 *
 * @return
 *  - Total number of bad blocks found.
 * --------------------------------------------------------------------------- */
uint32_t BBT_GetBadBlocks(uint32_t *list, uint32_t maxlen)
{
	uint32_t count = 0;

	for (uint32_t blk = 0; blk < TOTAL_BLOCKS; blk++)
	{
		if (BBT_Table[blk])
		{
			if (count < maxlen)
				list[count] = blk;
			count++;
		}
	}
	return count;
}

/* ===========================================================================
 * Function: FindFirstValidBlock
 * ===========================================================================
 * @brief
 *  - Finds the first valid (non-bad) block for allocation.
 *
 * @details
 *  - Scans from block index 8 to the start of factory reserved region.
 *  - Returns the first usable block that is not marked bad.
 *
 * @return
 *  - Block index of first valid block (≥ 8).
 *  - -1 if no valid block is found.
 *
 * @note
 *  - Reserved blocks 0–7 are typically used for boot or factory info.
 * --------------------------------------------------------------------------- */
int FindFirstValidBlock(void)
{
	for (int block = 8; block < FACTORY_INFO_BLOCK2_START; block++)
	{
		if (!BBT_IsBad(block))
		{
			return (int) block;
		}
	}

	return -1;
}
