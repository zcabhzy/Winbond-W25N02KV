/*
 *  Read_service.h
 *
 *  Created on: Sep 22, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_READ_SERVICE_H_
#define SERVICE_READ_SERVICE_H_

#include "nand_dri_Read.h"
#include "Protect_service.h"
#include "StatusRegister_service.h"

bool StandardRead_Service(uint32_t page_addr, uint16_t col_addr, uint8_t *buf,
		uint16_t len);
bool RandomRead_Service(uint32_t page_addr, uint16_t col_addr, uint8_t *buf,
		uint16_t len);

#endif /* SERVICE_READ_SERVICE_H_ */
