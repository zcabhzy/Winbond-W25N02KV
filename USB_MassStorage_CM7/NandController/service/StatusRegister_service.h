/*
 *  StatusRegister_service.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_STATUSREGISTER_SERVICE_H_
#define SERVICE_STATUSREGISTER_SERVICE_H_

#include "nand_dri_StatusRegister.h"

#define Status_Register1 0xA0
#define Status_Register2 0xB0
#define Status_Register3 0xC0

/**
 * Status Register-1 / Protection Register Setting
 * -----------------------------------------------
 * SRP0 - Status Register Protect-0
 * BP3  - Block Protect Bits
 * BP2  - Block Protect Bits
 * BP1  - Block Protect Bits
 * BP0  - Block Protect Bits
 * TB   - Top/Bottom Protect Bits
 * WP-E - /WP Enable Bit
 * SRP1 - Status Register Protect-1
 */
#define SR1_SRP0 (1u << 7) // S7
#define SR1_BP3  (1u << 6) // S6
#define SR1_BP2  (1u << 5) // S5
#define SR1_BP1  (1u << 4) // S4
#define SR1_BP0  (1u << 3) // S3
#define SR1_TB   (1u << 2) // S2
#define SR1_WPE  (1u << 1) // S1
#define SR1_SRP1 (1u << 0) // S0

uint8_t GetSR1(void);
uint8_t GetBlockProtectBits(void);  // 解析出 BP bits
bool IsTopBottomProtect(void);      // 是否啟用 Top/Bottom 保護
bool IsWriteProtectEnabled(void);   // 判斷 WP-E 狀態
bool IsSRP0Enabled(void);           // 判斷 SRP0 狀態
bool IsSRP1Enabled(void);           // 判斷 SRP1 狀態

/**
 * Status Register-2 / Protection Register Setting
 * -----------------------------------------------
 * OTP-L    - OTP Data Pages Lock
 * OTP-E    - Enter OTP Mode
 * SR1-L    - Status Register-1 Lock
 * ECC-E    - Enable ECC
 * BUF      - Buffer Mode
 * ODS[1:0] - Output Driver Strength
 * H-DIS    - Hold Disable
 */
#define SR2_OTPL (1u << 7) // S7
#define SR2_OTPE (1u << 6) // S6
#define SR2_SR1L (1u << 5) // S5
#define SR2_ECCE (1u << 4) // S4
#define SR2_BUF  (1u << 3) // S3
#define SR2_ODS1 (1u << 2) // S2
#define SR2_ODS0 (1u << 1) // S1
#define SR2_HDIS (1u << 0) // S0

// Output Driver Strength Decode(ODS[1:0])
#define SR2_ODS_MASK (SR2_ODS1 | SR2_ODS0)
#define SR2_ODS_SET1 (0b00)
#define SR2_ODS_SET2 (0b01)
#define SR2_ODS_SET3 (0b10)
#define SR2_ODS_SET4 (0b11)

uint8_t GetSR2(void);
bool GetOTP_Lock_service(void);             // OTP-L 狀態
bool IsOTPEnabled_service(void);               // OTP-E 狀態
bool IsSR1Locked_service(void);                // SR1-L 狀態
bool IsECCEnabled_service(void);               // ECC-E 狀態
bool IsBufferMode_service(void);               // BUF 狀態
uint8_t GetOutputDriverStrength_service(void); // ODS[1:0] 解碼
bool IsHoldDisabled_service(void);             // H-DIS 狀態

/**
 * Status Register-3 / Protection Register Setting
 * -----------------------------------------------
 * ECC-1  - ECC Status Bit[1:0]
 * ECC-0  - ECC Status Bit[1:0]
 * P-FAIL - Program Failure
 * E-FAIL - Erase Failure
 * WEL    - Write Enable Latch
 * BUSY   - Operation In Progress
 */
#define SR3_ECC2  (1u << 6) // S6
#define SR3_ECC1  (1u << 5) // S5
#define SR3_ECC0  (1u << 4) // S4
#define SR3_PFAIL (1u << 3) // S3
#define SR3_EFAIL (1u << 2) // S2
#define SR3_WEL   (1u << 1) // S1
#define SR3_BUSY  (1u << 0) // S0

// ECC Status Mask
#define SR3_ECC_MASK (SR3_ECC2 | SR3_ECC1 | SR3_ECC0)

typedef enum
{	ECC_SUCCESS = 0,
	ECC_SUCCESS_CORRECTED,
	ECC_UNCORRECTABLE,
	ECC_CORRECTED_THRESHOLD
} ECC_Status_t;

uint8_t GetSR3(void);
bool IsWriteEnableLatch_service(void);
bool IsBusy_service(void);
bool CheckProgramFail_service(void);
bool CheckEraseFail_service(void);
bool IsECCError_service(void);
ECC_Status_t GetECCStatus_service(void);

#endif /* SERVICE_STATUSREGISTER_SERVICE_H_ */
