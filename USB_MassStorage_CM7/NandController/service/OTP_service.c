/*
 *  OTP_service.c
 *
 *  Created on: Sep 23, 2025
 *  Author: User
 *  Folder: NandController/service
 */

#include "OTP_service.h"

/* ===========================================================================
 * Function: OTP_ReadPage
 * ===========================================================================
 * @brief
 *  - Reads data from a specified OTP (One-Time Programmable) page.
 *
 * @details
 *  - Executes a complete OTP page read flow using standard read commands
 *    while temporarily enabling OTP access mode.
 *  - The OTP region consists of 12 pages (0x00–0x0B) with fixed mapping:
 *      - Page 0x00 : Factory Unique ID
 *      - Page 0x01 : Parameter / Configuration data
 *      - Page 0x02–0x0B : User-programmable pages
 *  - Reading does not alter OTP contents and can be repeated indefinitely.
 *
 *  Command Flow:
 *    1. OTPEnable_Service(true)       → Enter OTP mode
 *    2. PageDataRead(13h)             → Transfer selected OTP page to cache
 *    3. IsBusyWithTimeout_service()   → Wait until device ready
 *    4. ReadData(03h)                 → Read from cache into host buffer
 *    5. OTPEnable_Service(false)      → Exit OTP mode
 *
 * @param otp_page_index : Target OTP page index (0x00–0x0B).
 * @param buf            : [out] Pointer to buffer for receiving data.
 * @param len            : Number of bytes to read.
 *
 * @return
 *  - true  : OTP page read completed successfully.
 *  - false : Invalid page index or read failure.
 *
 * @note
 *  - OTP access must be explicitly enabled before read and disabled afterward.
 *  - OTP pages 0x00–0x0B are non-volatile and write-once only.
 *  - Reference: Winbond W25N02KV Datasheet §7.7.1
 * --------------------------------------------------------------------------- */
bool OTP_ReadPage(uint8_t otp_page_index, uint8_t *buf, uint16_t len)
{
	if (otp_page_index > OTP_PAGE_MAX)
		return false;

	if (!OTPEnable_Service(true))
		return false;

	uint32_t page_addr = otp_page_index;

	PageDataRead(page_addr);

	if (!IsBusyWithTimeout_service(100))
		return false;

	ReadData(0x0000, buf, len);

	OTPEnable_Service(false);
	return true;
}

/* ===========================================================================
 * Function: OTP_ProgramPage
 * ===========================================================================
 * @brief
 *  - Programs user data into a specified OTP (One-Time Programmable) page.
 *
 * @details
 *  - Executes a complete OTP programming flow, combining standard NAND
 *    program commands under OTP mode.
 *  - Only pages 0x02–0x0B are user-writable; pages 0x00 and 0x01 are factory reserved.
 *  - Once programmed, data cannot be modified or erased.
 *
 *  Command Flow:
 *    1. OTPEnable_Service(true)       → Enter OTP mode
 *    2. LoadProgramData(02h)          → Load data into cache buffer
 *    3. ProgramExecute(10h)           → Commit cache data to OTP page
 *    4. IsBusyWithTimeout_service()   → Wait for program completion
 *    5. OTPEnable_Service(false)      → Exit OTP mode
 *
 * @param otp_page_index : Target OTP page index (0x02–0x0B).
 * @param buf            : [in] Pointer to data buffer to be programmed.
 * @param len            : Length of data to program into the page.
 *
 * @return
 *  - true  : OTP page successfully programmed.
 *  - false : Invalid page index, timeout, or program failure detected.
 *
 * @note
 *  - Requires Write Enable (WEL = 1) before programming.
 *  - OTP mode is volatile; it resets to normal mode on power cycle.
 *  - Programming time tPROG ≈ 200–600 µs.
 *  - Reference: Winbond W25N02KV Datasheet §7.7.2
 * --------------------------------------------------------------------------- */
bool OTP_ProgramPage(uint8_t otp_page_index, const uint8_t *buf, uint16_t len)
{
	if (otp_page_index < OTP_PAGE0 || otp_page_index > OTP_PAGE_MAX)
		return false;

	/// Step 1: Enable OTP
	if (!OTPEnable_Service(true))
		return false;

	uint32_t page_addr = otp_page_index;

	/// Step 2: Data -> Buffer => Buffer -> NAND
	LoadProgramData(0x0000, (uint8_t*) buf, len);
	ProgramExecute(page_addr);

	if (!IsBusyWithTimeout_service(100))
		return false;

	/// Step 1: Disable OTP
	OTPEnable_Service(false);
	return true;
}
