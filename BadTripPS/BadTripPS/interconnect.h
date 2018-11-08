#pragma once

#include <vector>
#include "bios.h"

#define START_CASH_DATA 0xbfc00000


struct _interconnect {
public:
	uint32_t load32(uint32_t addr);
};
	
