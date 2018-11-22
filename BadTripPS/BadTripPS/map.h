#pragma once

#include "bios.h"



struct _map
{
public:
	uint32_t map_x1;
	uint32_t map_x2;

	_map(uint32_t start, uint32_t end) :
		map_x1(start),
		map_x2(end)
	{};
	uint32_t contains(uint32_t addr);
};

struct _regIndex
{
	uint32_t m_index;
	_regIndex(uint32_t index) : m_index(index) {};
};


const _map CACHECONTROLL(0xfffe0130, 4);
const _map BIOS(0xbfc00000, 512 * 1024);
const _map MEMCONTROL(0x1f801000, 36);
const _map RAMSIZE(0x1f801060, 4);

