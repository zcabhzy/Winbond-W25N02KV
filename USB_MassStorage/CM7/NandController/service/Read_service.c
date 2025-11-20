/*
 *  Read_service.c
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "Read_service.h"

/* ===========================================================================
 * Function: StandardRead_Service
 * ===========================================================================
 * @brief
 *  - Executes a full-page standard read operation (13h + 03h).
 *
 * @details
 *  - Reads main data or full page content from the NAND array into MCU buffer.
 *  - Combines PageDataRead (13h) and ReadData (03h) operations under ECC monitoring.
 *  - Supports both ECC check and correction result reporting.
 *  - Requires ECC enabled in SR2 before calling this function.
 *  - Typical read latency tR ≈ 25–50 µs.
 *
 *  Command Flow:
 *    1. [13h + PA2:PA1:PA0]  PageDataRead → transfer page to cache
 *    2. Wait until OIP=0 (SR3[0]) → device ready
 *    3. [03h + CA1:CA0] ReadData → read from cache to host
 *    4. ECC check via GetECCStatus_service()
 *
 * @param page_addr : Target page address to read.
 * @param col_addr  : Column start address within page (0x0000 = main area).
 * @param buf       : [out] Pointer to buffer for receiving data.
 * @param len       : Number of bytes to read.
 *
 * @return
 *  - true  : Read success (ECC OK or corrected).
 *  - false : Timeout or ECC uncorrectable error.
 * --------------------------------------------------------------------------- */
bool StandardRead_Service(uint32_t page_addr, uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	PageDataRead(page_addr);

	if (!IsBusyWithTimeout_service(100))
	{
		printf("[Read] Timeout... Status is Busy.\r\n");
		return false;
	}

	ReadData(col_addr, buf, len);

	ECC_Status_t ecc = GetECCStatus_service();

	/// ECC Status check
	switch (ecc)
	{
	case ECC_SUCCESS:
		printf("[Status] ECC_SUCCESS (Page = 0x%05X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_SUCCESS_CORRECTED:
		printf(
				"[Status] ECC_SUCCESS_CORRECTED (Page = 0x%05X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_CORRECTED_THRESHOLD:
		printf(
				"[Status] ECC_CORRECTED_THRESHOLD (Page = 0x%05X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_UNCORRECTABLE:
		printf(
				"[Status] ECC_UNCORRECTABLE (Page = 0x%05X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;
	default:
		printf(
				"[Status] ECC_RESERVED  (Page = 0x%05X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		return false;
	}
	return true;
}

/* ===========================================================================
 * Function: RandomRead_Service
 * ===========================================================================
 * @brief
 *  - Executes a random column read sequence (13h + 03h with custom column).
 *
 * @details
 *  - Reads partial data (e.g., spare area or metadata) from a specific column
 *    offset in a given NAND page.
 *  - Performs ECC status check and classification after read.
 *
 *  Command Flow:
 *    1. [13h + PA2:PA1:PA0]  PageDataRead → load target page into cache
 *    2. Wait for OIP=0 (SR3[0]) → device ready
 *    3. [03h + CA1:CA0] ReadData → read from column offset
 *    4. ECC check via GetECCStatus_service()
 *
 * @param page_addr : Target page address to read.
 * @param col_addr  : Column address (0x0000–0x083F, main/spare area).
 * @param buf       : [out] Pointer to data buffer.
 * @param len       : Number of bytes to read.
 *
 * @return
 *  - true  : Read success (ECC OK or corrected).
 *  - false : Timeout or uncorrectable ECC error.
 * --------------------------------------------------------------------------- */
bool RandomRead_Service(uint32_t page_addr, uint16_t col_addr, uint8_t *buf, uint16_t len)
{
	/// Step 1: PageDataRead (0x13)
	PageDataRead(page_addr);

	/// Step 2: 等待 NAND Ready
	if (!IsBusyWithTimeout_service(100))
	{
		printf("[Random Read] Timeout ~ NAND Still Busy.\r\n");
		return false;
	}

	/// Step 3: ReadData with custom col_addr
	ReadData(col_addr, buf, len);

	ECC_Status_t ecc = GetECCStatus_service();

	/// ECC Status check
	switch (ecc)
	{
	case ECC_SUCCESS:
		printf("[Status] ECC_SUCCESS (Page = 0x%08X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_SUCCESS_CORRECTED:
		printf(
				"[Status] ECC_SUCCESS_CORRECTED (Page = 0x%08X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_CORRECTED_THRESHOLD:
		printf(
				"[Status] ECC_CORRECTED_THRESHOLD (Page = 0x%08X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		break;

	case ECC_UNCORRECTABLE:
	default:
		printf(
				"[Status] ECC_UNCORRECTABLE (Page = 0x%08X Col = 0x%04X Len = %u)\r\n",
				(unsigned int) page_addr, (unsigned int) col_addr,
				(unsigned int) len);
		return false;
	}

	return true;
}
