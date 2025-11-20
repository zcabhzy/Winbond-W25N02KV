/*
 *  Program_service.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_PROGRAM_SERVICE_H_
#define SERVICE_PROGRAM_SERVICE_H_

#include "nand_dri_Program.h"
#include "Protect_service.h"
#include "StatusRegister_service.h"

bool StandardProgram_Service(uint32_t page_addr, const uint8_t *buf,
		uint16_t len);

bool RandomProgram_Service(uint32_t page_addr, uint16_t col_addr,
		const uint8_t *buf, uint16_t len);

#endif /* SERVICE_PROGRAM_SERVICE_H_ */
