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
	uint32_t contains(const uint32_t addr);
};

struct _regIndex
{
	uint32_t m_index;
	_regIndex(uint32_t index) : m_index(index) {};
};





const uint32_t REGION_MASK[8] = {
	//KUSEG: 2048MB
	0xfffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	//KSEG0 : 512 Mb
	0x7ffffff,
	//KSEG1: 512 Mb
	0x1ffffff,
	//KSEG2: 1024Mb
	0xfffffff, 0xfffffff 
};

namespace pscx_memory {
	_map CACHECONTROLL(0xfffe0130, 4);
	_map BIOS(0x1fc00000, 512 * 1024);
	_map MEMCONTROL(0x1f801000, 36);
	_map RAMSIZE(0x1f801060, 4);
	_map RAM(0x00000000, 2 * 1024 * 1024);
	_map SYSCONTROL(0x1f801000, 36);
	_map SPU(0x1f801c00, 640);
	_map EXPANSION2(0x1f802000, 66);
	_map EXPANSION1(0x1f000000, 512 * 1024);
	_map IRQCONTROL(0x1f801070, 8);

	uint32_t mask_region(uint32_t addr) {
		size_t index = addr >> 29;
		return addr & REGION_MASK[index];
	};
}