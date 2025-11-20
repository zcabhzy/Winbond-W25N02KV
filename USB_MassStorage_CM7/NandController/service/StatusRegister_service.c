/*
 *  StatusRegister_service.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "StatusRegister_service.h"

/// =========================== Status Register 1 ============================

/* ---------------------------------------------------------------------------
 * Function: GetSR1
 * ---------------------------------------------------------------------------
 * @brief
 *  - Read Status Register-1 (Address = 0xA0).
 *
 * @details
 *  - Contains protection configuration bits:
 *      BP[3:0] : Block Protect region
 *      TB      : Top/Bottom protect mode
 *      WP-E    : Write Protect Enable
 *      SRP[1:0]: Status Register Protect bits
 *
 * @return
 *  - 8-bit raw SR1 value.
 * --------------------------------------------------------------------------- */
uint8_t GetSR1(void)
{
	return ReadStatusRegister(Status_Register1);
}

/* ---------------------------------------------------------------------------
 * Function: GetBlockProtectBits
 * ---------------------------------------------------------------------------
 * @brief
 *  - Extract Block Protect bits (BP[3:0]) from SR1.
 * @return
 *  - 4-bit value (0–15) indicating the protected memory range.
 * --------------------------------------------------------------------------- */
uint8_t GetBlockProtectBits(void)
{
	return (GetSR1() & (SR1_BP3 | SR1_BP2 | SR1_BP1 | SR1_BP0)) >> 3;
}

/* ---------------------------------------------------------------------------
 * Function: IsTopBottomProtect
 * ---------------------------------------------------------------------------
 * @brief
 *  - Check TB bit (Top/Bottom protect selection).
 * @return
 *  - true  : Top area protected (TB=1)
 *  - false : Bottom area protected (TB=0)
 * --------------------------------------------------------------------------- */
bool IsTopBottomProtect(void)
{
	return (GetSR1() & SR1_TB) ? true : false;
}

/* ---------------------------------------------------------------------------
 * Function: IsWriteProtectEnabled
 * ---------------------------------------------------------------------------
 * @brief
 *  - Check Write Protect Enable (WP-E) bit in SR1.
 * @return
 *  - true  : WP-E = 1 (write protected)
 *  - false : WP-E = 0
 * --------------------------------------------------------------------------- */
bool IsWriteProtectEnabled(void)
{
	return (GetSR1() & SR1_WPE) ? true : false;
}

/* ---------------------------------------------------------------------------
 * Function: IsSRP0Enabled / IsSRP1Enabled
 * ---------------------------------------------------------------------------
 * @brief
 *  - Check Status Register Protect bits (SRP0, SRP1).
 * @return
 *  - true  : bit set
 *  - false : bit clear
 * --------------------------------------------------------------------------- */
bool IsSRP0Enabled(void)
{
	return (GetSR1() & SR1_SRP0) ? true : false;
}

bool IsSRP1Enabled(void)
{
	return (GetSR1() & SR1_SRP1) ? true : false;
}

/// =========================== Status Register 2 ============================

/* ---------------------------------------------------------------------------
 * Function: GetSR2
 * ---------------------------------------------------------------------------
 * @brief
 *  - Read Status Register-2 (Address = 0xB0).
 *
 * @details
 *  - Contains configuration features:
 *      OTPL : OTP area lock
 *      OTPE : OTP enable
 *      SR1L : SR1 lock
 *      ECCE : ECC enable
 *      BUF  : Buffer mode
 *      ODS[2:0] : Output driver strength
 *      HDIS : HOLD# disable
 *
 * @return
 *  - 8-bit raw SR2 value.
 * --------------------------------------------------------------------------- */
uint8_t GetSR2(void)
{
	return ReadStatusRegister(Status_Register2);
}

/* ---------------------------------------------------------------------------
 * Individual SR2 bit accessors
 * --------------------------------------------------------------------------- */
bool GetOTP_Lock_service(void)
{
	return (GetSR2() & SR2_OTPL) ? true : false;
}

bool IsOTPEnabled_service(void)
{
	return (GetSR2() & SR2_OTPE) ? true : false;
}

bool IsSR1Locked_service(void)
{
	return (GetSR2() & SR2_SR1L) ? true : false;
}

bool IsECCEnabled_service(void)
{
	return (GetSR2() & SR2_ECCE) ? true : false;
}

bool IsBufferMode_service(void)
{
	return (GetSR2() & SR2_BUF) ? true : false;
}

uint8_t GetOutputDriverStrength_service(void)
{
	return (GetSR2() & SR2_ODS_MASK) >> 1;
}

bool IsHoldDisabled_service(void)
{
	return (GetSR2() & SR2_HDIS) ? true : false;
}

/// ============================== Status Register 3 ==============================

/// -------------------------------------------------------------------------------
/**
 *  @brief  Read Status Register-3.
 *
 *  @details
 *  - Command: 0x05 + Address = 0xC0
 *  - Contains operation status bits: ECC, P-FAIL, E-FAIL, WEL, and BUSY.
 *
 *  @return 8-bit value of SR3.
 */
/// -------------------------------------------------------------------------------
uint8_t GetSR3(void)
{
	return ReadStatusRegister(Status_Register3);
}

/* ---------------------------------------------------------------------------
 * Function: IsWriteEnableLatch_service / IsBusy_service
 * ---------------------------------------------------------------------------
 * @brief
 *  - Read dynamic operation bits.
 * @return
 *  - true if bit is set, false otherwise.
 * --------------------------------------------------------------------------- */
bool IsWriteEnableLatch_service(void)
{
	return (GetSR3() & SR3_WEL) ? true : false;
}

bool IsBusy_service(void)
{
	return (GetSR3() & SR3_BUSY) ? true : false;
}

/* ---------------------------------------------------------------------------
 * Function: CheckProgramFail_service / CheckEraseFail_service
 * ---------------------------------------------------------------------------
 * @brief
 *  - Detect program or erase failure from SR3 flags.
 * @return
 *  - true if respective fail bit = 1.
 * --------------------------------------------------------------------------- */
bool CheckProgramFail_service(void)
{
	return (GetSR3() & SR3_PFAIL) ? true : false;
}

bool CheckEraseFail_service(void)
{
	return (GetSR3() & SR3_EFAIL) ? true : false;
}

/* ---------------------------------------------------------------------------
 * Function: IsECCError_service
 * ---------------------------------------------------------------------------
 * @brief
 *  - Determine if ECC error condition exists.
 * @return
 *  - true  : Uncorrectable or threshold-corrected error detected.
 *  - false : No ECC issue.
 * --------------------------------------------------------------------------- */
bool IsECCError_service(void)
{
	ECC_Status_t ecc_status = GetECCStatus_service();

	return (ecc_status == ECC_UNCORRECTABLE
			|| ecc_status == ECC_CORRECTED_THRESHOLD);
}

/* ---------------------------------------------------------------------------
 * Function: GetECCStatus_service
 * ---------------------------------------------------------------------------
 * @brief
 *  - Decode ECC result bits (SR3[6:4]).
 *
 * @details
 *  - ECC field definition (per W25N02KV §7.6):
 *      000 : No error (ECC_SUCCESS)
 *      001 : Corrected error (ECC_SUCCESS_CORRECTED)
 *      010 : Uncorrectable (ECC_UNCORRECTABLE)
 *      011 : Corrected, threshold reached (ECC_CORRECTED_THRESHOLD)
 *  - Used after each read operation for reliability check.
 *
 * @return
 *  - ECC_Status_t : Enumerated result.
 * --------------------------------------------------------------------------- */
ECC_Status_t GetECCStatus_service(void)
{
	uint8_t sr3 = GetSR3();
	uint8_t ecc_bits = (sr3 & SR3_ECC_MASK) >> 4;

	switch (ecc_bits)
	{
	case 0b000:
		return ECC_SUCCESS;
	case 0b001:
		return ECC_SUCCESS_CORRECTED;
	case 0b010:
		return ECC_UNCORRECTABLE;
	case 0b011:
		return ECC_CORRECTED_THRESHOLD;
	default:
		printf("[Warning] ECC Reserved code : 0b%03u (SR3 = 0x%02X)\r\n",
				ecc_bits, sr3);
		return ECC_SUCCESS;
	}
}
