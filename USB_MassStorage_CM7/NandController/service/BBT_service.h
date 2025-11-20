/*
 *  BBT_service.h
 *
 *  Created on: Sep 24, 2025
 *  Author: Henry
 *  Folder: NandController/service
 */

#ifndef SERVICE_BBT_SERVICE_H_
#define SERVICE_BBT_SERVICE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "W25N02KV_Config.h"
#include "nand_dri_Program.h"
#include "Read_service.h"
#include "Program_service.h"

/* Global BBT Table */
extern bool BBT_Table[TOTAL_BLOCKS];

/* Core Management */
void BBT_ScanFactoryBlocks(void);
void BBT_MarkRuntimeBad(uint32_t block);

/* Query / Output */
bool BBT_IsBad(uint32_t block);
void BBT_PrintSummary(void);
void BBT_ShowBadBlock(void);
uint32_t BBT_GetBadBlocks(uint32_t *list, uint32_t maxlen);

int FindFirstValidBlock(void);

/* Inline Helper */
static inline bool BBT_BadBlock(uint32_t block)
{
	return (block >= TOTAL_BLOCKS) ? true : BBT_Table[block];
}

#endif /* SERVICE_BBT_SERVICE_H_ */
