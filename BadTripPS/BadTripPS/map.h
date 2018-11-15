#pragma once

#include "bios.h"

struct _map
{
public:
	uint32_t map_x1;
	uint32_t map_x2;

	_map(uint32_t start,uint32_t end):
		map_x1(start),
		map_x2(end)
	{};
	uint32_t contains(uint32_t addr);
};

struct _regIndex
{
	_regIndex(uint32_t index) : m_index(index) {};
	uint32_t m_index;
};

