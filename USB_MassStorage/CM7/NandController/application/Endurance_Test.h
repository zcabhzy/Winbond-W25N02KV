/*
 *  EnduranceTest.h
 *
 *  Created on: Oct 5, 2025
 *  Author: Henry
 *  Folder: NandController/application
 */

#ifndef APPLICATION_ENDURANCE_TEST_H_
#define APPLICATION_ENDURANCE_TEST_H_

#include "W25N02KV_Config.h"
#include "Pattern.h"

#include "BBT_service.h"
#include "Read_service.h"
#include "Reset_service.h"
#include "Program_service.h"
#include "BlockErase_service.h"

#define MAX_PE_CYCLE 60000

void EnduranceTest_Run(uint32_t block);

#endif /* APPLICATION_ENDURANCE_TEST_H_ */
