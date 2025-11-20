/*
 *  OTP_service.h
 *
 *  Created on: Sep 23, 2025
 *  Author: User
 *  Folder: NandController/service
 */

#ifndef SERVICE_OTP_SERVICE_H_
#define SERVICE_OTP_SERVICE_H_

#include "nand_dri_Read.h"
#include "nand_dri_Program.h"
#include "Protect_service.h"
#include "StatusRegister_service.h"

#define OTP_PAGE_UNIQUE_ID   0x00
#define OTP_PAGE_PARAMETER   0x01
#define OTP_PAGE0            0x02
#define OTP_PAGE_MAX         0x0B

bool OTP_ReadPage(uint8_t otp_page_index, uint8_t *buf, uint16_t len);
bool OTP_ProgramPage(uint8_t otp_page_index, const uint8_t *buf, uint16_t len);

#endif /* SERVICE_OTP_SERVICE_H_ */
