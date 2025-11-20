/*
 *  BlockErase_service.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_BLOCKERASE_SERVICE_H_
#define SERVICE_BLOCKERASE_SERVICE_H_

#include "Protect_service.h"
#include "nand_dri_BlockErase.h"
#include "StatusRegister_service.h"

// 建議 Timeout
#define BLOCK_ERASE_TIMEOUT_MS   500

bool BlockErase128K_service(uint32_t block_addr, uint32_t timeout_ms);

#endif /* SERVICE_BLOCKERASE_SERVICE_H_ */
