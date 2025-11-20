/*
 *  Pattern.c
 *
 *  Created on: Oct 14, 2025
 *  Author: Henry
 *  Folder: NandController/Parrten
 */

#include "Pattern.h"

/* ===========================================================================
 * Function: PreparePattern
 * ===========================================================================
 * @brief
 *
 * @details
 *
 * @param
 *
 * @return
 * --------------------------------------------------------------------------- */
void PreparePattern(uint8_t *buf, uint32_t len, PatternType_t type)
{
	switch (type)
	{
	case PATTERN_AA:
		memset(buf, 0xAA, len);
		break;

	case PATTERN_55:
		memset(buf, 0x55, len);
		break;

	case PATTERN_FF:
		memset(buf, 0xFF, len);
		break;

	case PATTERN_00:
		memset(buf, 0x00, len);
		break;

	case PATTERN_ALT_AA55:
		for (uint32_t i = 0; i < len; i++)
			buf[i] = (i % 2 == 0) ? 0xAA : 0x55;
		break;

	case PATTERN_SEQ_AA55FF00:
	{
		uint8_t seq[] = { 0xAA, 0x55, 0xFF, 0x00 };
		for (uint32_t i = 0; i < len; i++)
			buf[i] = seq[i % 4];
		break;
	}

	case PATTERN_PRBS:
	{
		uint32_t lfsr = 0xACE1u;  // 初始種子，可修改
		for (uint32_t i = 0; i < len; i++)
		{
			// LFSR (x^16 + x^14 + x^13 + x^11) 多項式
			lfsr = (lfsr >> 1) ^ (-(int32_t) (lfsr & 1u) & 0xB400u);
			buf[i] = (uint8_t) lfsr;
		}
		break;
	}

	default:
		memset(buf, 0x00, len);
		break;
	}
}
