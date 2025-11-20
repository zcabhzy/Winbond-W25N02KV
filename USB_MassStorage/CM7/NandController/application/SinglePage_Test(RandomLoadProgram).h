/*
 *  SinglePage_Test(RandomLoadProgram).h
 *
 *  Created on: Oct 14, 2025
 *  Author: CPC
 */

#ifndef APPLICATION_SINGLEPAGE_TEST_RANDOMLOADPROGRAM__H_
#define APPLICATION_SINGLEPAGE_TEST_RANDOMLOADPROGRAM__H_

#include "W25N02KV_Config.h"
#include "Pattern.h"

#include "BBT_service.h"
#include "Read_service.h"
#include "Reset_service.h"
#include "Program_service.h"
#include "BlockErase_service.h"

void Random_UnitTest(uint32_t block_num);

#endif /* APPLICATION_SINGLEPAGE_TEST_RANDOMLOADPROGRAM__H_ */
