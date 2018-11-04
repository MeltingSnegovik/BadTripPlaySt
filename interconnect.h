#pragma once

#include <vector>
#include "bios.h"

#define START_CASH_DATA 0xbfc00000
struct _interconnect {
	_bios bios;
	_interconnect(_bios t_bios):
		bios(t_bios);
	uint32_t Load32(uint32_t addr);
}
	
