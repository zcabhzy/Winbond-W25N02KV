/*
 *  Pattern.h
 *
 *  Created on: Oct 14, 2025
 *  Author: CPC
 *  Folder: NandController/Parrten
 */

#ifndef APPLICATION_PATTERN_H_
#define APPLICATION_PATTERN_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "W25N02KV_Config.h"

typedef enum
{
	PATTERN_AA,
	PATTERN_55,
	PATTERN_FF,
	PATTERN_00,
	PATTERN_ALT_AA55,
	PATTERN_SEQ_AA55FF00,
	PATTERN_PRBS
} PatternType_t;

void PreparePattern(uint8_t *buf, uint32_t len, PatternType_t type);

#endif /* APPLICATION_PATTERN_H_ */
