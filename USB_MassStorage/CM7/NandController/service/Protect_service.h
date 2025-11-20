/*
 *  Protect_service.h
 *
 *  Created on: Sep 20, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_PROTECT_SERVICE_H_
#define SERVICE_PROTECT_SERVICE_H_

#include "nand_dri_Protect.h"
#include "StatusRegister_service.h"

bool WriteEnable_Service(void);
void WriteDisable_Service(void);

/* ==================== Status Register 1 ==================== */

bool SetBlockProtect_Service(uint8_t bp_value, bool top_bottom);
bool SetWriteProtectEnable_Service(bool enable);
bool SetStatusRegisterProtect_Service(bool srp0, bool srp1);

/* ==================== Status Register 2 ==================== */

bool OTPLock_Service(void);              			 // OTP-L (一次性鎖定)
bool OTPEnable_Service(bool enable);     			 // OTP-E
bool SR1Lock_Service(void);              			 // SR1-L (一次性鎖定)
bool SetECCEnable_Service(bool enable);     		 // ECC-E
bool SetBufferMode_Service(bool enable);    		 // BUF
bool SetOutputDriverStrength_Service(uint8_t level); // ODS[1:0]
bool SetHoldDisable_Service(bool disable);  		 // H-DIS
bool SetReadMode_Service(bool buf, bool ecc);

/* ==================== Status Register 3 ==================== */

bool IsBusyWithTimeout_service(uint32_t timeout_ms);

#endif /* SERVICE_PROTECT_SERVICE_H_ */
