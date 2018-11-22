#pragma once

#include <vector>
#include "bios.h"
#include "map.h"

#define START_CASH_DATA 0xbfc00000


struct _interconnect {
public:
	
	_bios Bios;
	_map MemControll;

	_interconnect(_bios bios) :
		Bios(bios),
		MemControll(0x1f801000,36)
	{
	};

	uint32_t load32(uint32_t addr);
	void Store32(uint32_t addr, uint32_t val);
	uint32_t Load32(uint32_t addr);
	void Store16(uint32_t addr, uint16_t val);
	void Store8(uint32_t addr, uint8_t val);
	uint8_t Load8(uint32_t addr);
};
	
