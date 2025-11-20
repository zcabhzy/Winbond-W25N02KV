/*
 *  Protect_service.c
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#include "Protect_service.h"

/* ===========================================================================
 * Function: WriteEnable_Service
 * ===========================================================================
 * @brief
 *  - Issues WRITE ENABLE (06h) command and verifies that WEL = 1.
 *
 * @details
 *  - Sets the Write Enable Latch (WEL) bit in Status Register 1 (SR1[1]).
 *  - Required before any program, erase, or status register modification.
 *  - Performs verification by reading SR1 and confirming WEL is asserted.
 *
 * @return
 *  - true  : WEL = 1 (Write enable success)
 *  - false : WEL = 0 (Write enable failed)
 * --------------------------------------------------------------------------- */
bool WriteEnable_Service(void)
{
	WriteEnable();

	if (IsWriteEnableLatch_service())
	{
		printf("[Write Enable] Write Enable Success (WEL = 1)\r\n");
		return true;
	}
	else
	{
		printf("[Write Enable] Write Enable Failed  (WEL = 0)\r\n");
		return false;
	}
}

/* ===========================================================================
 * Function: WriteDisable_Service
 * ===========================================================================
 * @brief
 *  - Issues WRITE DISABLE (04h) command and verifies that WEL = 0.
 *
 * @details
 *  - Clears the Write Enable Latch (WEL) bit to block further write/erase.
 *  - Used after sensitive operations to prevent accidental modification.
 * --------------------------------------------------------------------------- */
void WriteDisable_Service(void)
{
	WriteDisable();

	if (!IsWriteEnableLatch_service())
		printf("[Write Disable] Write Disable Success (WEL = 0)\r\n");
	else
		printf("[Write Disable] Write Disable Failed  (WEL = 1)\r\n");
}

/// =========================== Status Register 1 ============================

/* ===========================================================================
 * Function: SetBlockProtect_Service
 * ===========================================================================
 * @brief
 *  - Configures block protection range (BP[3:0]) and direction (TB) in SR1.
 *
 * @details
 *  - BP[3:0]: Defines the protected region size (top or bottom of array).
 *  - TB: Selects protection direction (Top = 1 / Bottom = 0).
 *  - Writes SR1 with the new BP/TB configuration and verifies correctness.
 *  - Requires Write Enable before updating SR1.
 *
 * @param bp_value   : Block protect value (0–15)
 * @param top_bottom : true = Top protected (TB=1), false = Bottom protected (TB=0)
 *
 * @return
 *  - true  : Operation verified successfully
 *  - false : Verification mismatch or write failure
 * --------------------------------------------------------------------------- */
bool SetBlockProtect_Service(uint8_t bp_value, bool top_bottom)
{
	uint8_t sr1 = GetSR1();

	sr1 &= ~(SR1_BP3 | SR1_BP2 | SR1_BP1 | SR1_BP0 | SR1_TB);
	sr1 |= ((bp_value & 0x0F) << 3);

	if (top_bottom)
		sr1 |= SR1_TB;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register1, sr1);

	if (GetBlockProtectBits() == bp_value && IsTopBottomProtect() == top_bottom)
	{
		printf("[Set Block Protect] Success (BP=0x%X, TB=%u)\r\n",
				GetBlockProtectBits(), IsTopBottomProtect());
		return true;
	}
	else
	{
		printf(
				"[Set Block Protect] Failed (Expect BP=0x%X, TB=%u / Got BP=0x%X, TB=%u)\r\n",
				bp_value, top_bottom, GetBlockProtectBits(),
				IsTopBottomProtect());
		return false;
	}
}

/* ===========================================================================
 * Function: SetWriteProtectEnable_Service
 * ===========================================================================
 * @brief
 *  - Enables or disables the Write Protect Enable (WP-E) bit in SR1.
 *
 * @param enable : true = WP-E enabled, false = WP-E disabled
 *
 * @return
 *  - true  : Operation verified successfully
 *  - false : Write or verification failed
 * --------------------------------------------------------------------------- */
bool SetWriteProtectEnable_Service(bool enable)
{
	uint8_t sr1 = GetSR1();

	if (enable)
		sr1 |= SR1_WPE;
	else
		sr1 &= ~SR1_WPE;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register1, sr1);

	if (IsWriteProtectEnabled() == enable)
	{
		printf("[SET WP-E] Success (WP-E = %u)\r\n", IsWriteProtectEnabled());
		return true;
	}
	else
	{
		printf("[SET WP-E] Failed (Expect WP-E = %u / Got WP-E = %u)\r\n",
				enable, IsWriteProtectEnabled());
		return false;
	}
}

/* ===========================================================================
 * Function: SetStatusRegisterProtect_Service
 * ===========================================================================
 * @brief
 *  - Configures SRP0 and SRP1 bits to control write protection of SR registers.
 *
 * @details
 *  - SRP bits determine whether status registers can be modified.
 *  - When protection is enabled, hardware WP# may be required to unlock.
 *
 * @param srp0 : SRP0 bit value
 * @param srp1 : SRP1 bit value
 *
 * @return
 *  - true  : Operation verified successfully
 *  - false : Write or verification failed
 * --------------------------------------------------------------------------- */
bool SetStatusRegisterProtect_Service(bool srp0, bool srp1)
{
	uint8_t sr1 = GetSR1();

	// Clear old SRP bits
	sr1 &= ~(SR1_SRP0 | SR1_SRP1);

	if (srp0)
		sr1 |= SR1_SRP0;
	if (srp1)
		sr1 |= SR1_SRP1;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register1, sr1);

	if (IsSRP0Enabled() == srp0 && IsSRP1Enabled() == srp1)
	{
		printf("[SET SRP] Success (SRP0 = %u, SRP1 = %u)\r\n", IsSRP0Enabled(),
				IsSRP1Enabled());
		return true;
	}
	else
	{
		printf(
				"[SET SRP] Failed (Expect SRP0 = %u, SRP1 = %u / Got SRP0 = %u, SRP1 = %u)\r\n",
				srp0, srp1, IsSRP0Enabled(), IsSRP1Enabled());
		return false;
	}
}

/// =========================== Status Register 2 ============================

/* ===========================================================================
 * Function: OTPLock_Service
 * ===========================================================================
 * @brief
 *  - Permanently locks the OTP area by setting OTPL=1 in SR2.
 *
 * @details
 *  - Once set, the OTP area becomes read-only and cannot be reprogrammed.
 *  - This operation is irreversible.
 *
 * @return
 *  - true  : OTP lock success (OTPL=1)
 *  - false : Write or verification failed
 * --------------------------------------------------------------------------- */
bool OTPLock_Service(void)
{
	uint8_t sr2 = GetSR2();
	sr2 |= SR2_OTPL;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (GetOTP_Lock_service())
	{
		printf("[SET OTP-L] Success (OTP-L = 1, Locked permanently)\r\n");
		return true;
	}
	else
	{
		printf("[SET OTP-L] Failed (OTP-L not set)\r\n");
		return false;
	}
}

/* ===========================================================================
 * Function: OTPEnable_Service
 * ===========================================================================
 * @brief
 *  - Enables or disables OTP mode via the OTP-E bit (SR2[0]).
 *
 * @param enable : true = Enable OTP mode, false = Disable OTP mode
 *
 * @return
 *  - true  : Verified success
 *  - false : Verification mismatch or write failure
 * --------------------------------------------------------------------------- */
bool OTPEnable_Service(bool enable)
{
	uint8_t sr2 = GetSR2();

	if (enable)
		sr2 |= SR2_OTPE;
	else
		sr2 &= ~SR2_OTPE;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (IsOTPEnabled_service() == enable)
	{
		printf("[SET OTP-E] Success (OTP-E = %u)\r\n", IsOTPEnabled_service());
		return true;
	}
	else
	{
		printf("[SET OTP-E] Failed (Expect OTP-E = %u / Got OTP-E = %u)\r\n",
				enable, IsOTPEnabled_service());
		return false;
	}
}

/* ===========================================================================
 * Function: SR1Lock_Service
 * ===========================================================================
 * @brief
 *  - Permanently locks Status Register 1 (SR1) by setting SR1-L bit (SR2[7]).
 *
 * @details
 *  - Once locked, SR1 cannot be modified again.
 *  - This action is irreversible.
 * --------------------------------------------------------------------------- */
bool SR1Lock_Service(void)
{
	uint8_t sr2 = GetSR2();
	sr2 |= SR2_SR1L;  // SR1-L = 1 (永久鎖定)

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (IsSR1Locked_service())
	{
		printf("[SET SR1-L] Success (SR1-L = 1, Locked permanently)\r\n");
		return true;
	}
	else
	{
		printf("[SET SR1-L] Failed (SR1-L not set)\r\n");
		return false;
	}
}

/* ===========================================================================
 * Function: SetECCEnable_Service
 * ===========================================================================
 * @brief
 *  - Enables or disables internal ECC engine (ECC-E bit in SR2).
 *
 * @param enable : true = Enable ECC, false = Disable ECC
 *
 * @return
 *  - true  : Operation verified successfully
 *  - false : Verification mismatch or write failure
 * --------------------------------------------------------------------------- */
bool SetECCEnable_Service(bool enable)
{
	uint8_t sr2 = GetSR2();

	if (enable)
		sr2 |= SR2_ECCE;
	else
		sr2 &= ~SR2_ECCE;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (IsECCEnabled_service() == enable)
	{
		printf("[SET ECC] Success (ECC-E = %u)\r\n", IsECCEnabled_service());
		return true;
	}
	else
	{
		printf("[SET ECC] Failed (Expect ECC-E = %u / Got ECC-E = %u)\r\n",
				enable, IsECCEnabled_service());
		return false;
	}
}

/* ===========================================================================
 * Function: SetBufferMode_Service
 * ===========================================================================
 * @brief
 *  - Enables or disables buffer read mode (BUF bit in SR2).
 *
 * @param enable : true = Enable buffer mode, false = Disable buffer mode
 *
 * @return
 *  - true  : Verified success
 *  - false : Verification failed
 * --------------------------------------------------------------------------- */
bool SetBufferMode_Service(bool enable)
{
	uint8_t sr2 = GetSR2();

	if (enable)
		sr2 |= SR2_BUF;
	else
		sr2 &= ~SR2_BUF;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (IsBufferMode_service() == enable)
	{
		printf("[SET BUF] Success (BUF = %u)\r\n", IsBufferMode_service());
		return true;
	}
	else
	{
		printf("[SET BUF] Failed (Expect BUF = %u / Got BUF = %u)\r\n", enable,
				IsBufferMode_service());
		return false;
	}
}

/* ===========================================================================
 * Function: SetOutputDriverStrength_Service
 * ===========================================================================
 * @brief
 *  - Configures output driver strength (ODS[2:0]) in SR2.
 *
 * @param level : Drive strength level (0–3)
 *
 * @return
 *  - true  : Operation verified successfully
 *  - false : Invalid level or verification failed
 * --------------------------------------------------------------------------- */
bool SetOutputDriverStrength_Service(uint8_t level)
{
	if (level > 3)
		return false;

	uint8_t sr2 = GetSR2();

	sr2 &= ~SR2_ODS_MASK;
	sr2 |= (level << 1);

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	if (GetOutputDriverStrength_service() == level)
	{
		printf("[SET ODS] Success (ODS = %u)\r\n",
				GetOutputDriverStrength_service());
		return true;
	}
	else
	{
		printf("[SET ODS] Failed (Expect ODS = %u / Got ODS = %u)\r\n", level,
				GetOutputDriverStrength_service());
		return false;
	}
}

/* ===========================================================================
 * Function: SetHoldDisable_Service
 * ===========================================================================
 * @brief
 *  - Enables or disables HOLD# pin function (H-DIS bit in SR2).
 *
 * @param disable : true = Disable HOLD#, false = Enable HOLD#
 *
 * @return
 *  - true  : Verified success
 *  - false : Verification mismatch or write failure
 * --------------------------------------------------------------------------- */
bool SetHoldDisable_Service(bool disable)
{
	uint8_t sr2 = GetSR2();

	if (disable)
		sr2 |= SR2_HDIS;
	else
		sr2 &= ~SR2_HDIS;

	if (!WriteEnable_Service())
	{
		return false;
	}

	WriteStatusRegister(Status_Register2, sr2);

	if (IsHoldDisabled_service() == disable)
	{
		printf("[SET H-DIS] Success (H-DIS = %u)\r\n",
				IsHoldDisabled_service());
		return true;
	}
	else
	{
		printf("[SET H-DIS] Failed (Expect H-DIS = %u / Got H-DIS = %u)\r\n",
				disable, IsHoldDisabled_service());
		return false;
	}
}

/* ===========================================================================
 * Function: SetReadMode_Service
 * ===========================================================================
 * @brief
 *  - Configures read mode by setting both BUF and ECC-E bits in SR2.
 *
 * @param buf : true = Enable buffer mode
 * @param ecc : true = Enable ECC
 *
 * @return
 *  - true  : Verified success
 *  - false : Verification failed
 * --------------------------------------------------------------------------- */
bool SetReadMode_Service(bool buf, bool ecc)
{
	uint8_t sr2 = GetSR2();

	// 清除原有 BUF 與 ECC-E
	sr2 &= ~(SR2_BUF | SR2_ECCE);

	// 設定新的值
	if (buf)
		sr2 |= SR2_BUF;
	if (ecc)
		sr2 |= SR2_ECCE;

	if (!WriteEnable_Service())
		return false;

	WriteStatusRegister(Status_Register2, sr2);

	// 驗證
	bool new_buf = IsBufferMode_service();
	bool new_ecc = IsECCEnabled_service();

	if (new_buf == buf && new_ecc == ecc)
	{
		printf("[SET ReadMode] Success (BUF=%u, ECC-E=%u)\r\n", new_buf,
				new_ecc);
		return true;
	}
	else
	{
		printf(
				"[SET ReadMode] Failed (Expect BUF=%u,ECC-E=%u / Got BUF=%u,ECC-E=%u)\r\n",
				buf, ecc, new_buf, new_ecc);
		return false;
	}
}

/// =========================== Status Register 3 ============================

/* ===========================================================================
 * Function: IsBusyWithTimeout_service
 * ===========================================================================
 * @brief
 *  - Polls the OIP (Operation In Progress) bit in SR3 until cleared or timeout.
 *
 * @details
 *  - Returns immediately when device becomes ready (OIP=0) or timeout expires.
 *  - Used to synchronize host operations after program/erase commands.
 *
 * @param timeout_ms : Timeout in milliseconds
 *
 * @return
 *  - true  : Device ready (OIP=0)
 *  - false : Timeout expired while device busy
 *
 * @note
 *  - Reference: Winbond W25N02KV Datasheet §7.6.3
 * --------------------------------------------------------------------------- */
bool IsBusyWithTimeout_service(uint32_t timeout_ms)
{
	uint32_t start = HAL_GetTick();

	while ((HAL_GetTick() - start) < timeout_ms)
	{
		if (!IsBusy_service())
		{
			printf("[OIP Status: 0] device is ready\r\n");
			return true;
		}
	}

	printf("[OIP Status: 1] Timeout, device still busy\r\n");
	return false;
}
