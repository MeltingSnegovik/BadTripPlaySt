#pragma once

#include "bios.h"
#include <iostream>

namespace pscx_memory {

	enum _port {
		//Macroblock decoder input
		e_MdecIn = 0,
		//Macroblock decoder output
		e_MdecOut = 1,
		//Graphics Processing Unit
		e_Gpu = 2,
		//Cd-rom drive
		e_CdRom = 3,
		//Sound processing Unit
		e_Spu = 4,
		//Extension Port		
		e_Pio = 5,
		//used to clear the ordering table
		e_Otc = 6
	};


	struct _map
	{
	public:
		uint32_t map_x1;
		uint32_t map_x2;

		_map(uint32_t start, uint32_t end) :
			map_x1(start),
			map_x2(end)
		{};
		uint32_t contains(uint32_t addr) const;
	};

	struct _regIndex
	{
		uint32_t m_index;
		_regIndex(uint32_t index) : m_index(index) {};
	};

	struct _buffer {
		uint32_t m_buffer[12];
		uint8_t m_len;

		_buffer() :
			m_len(0)
		{
			memset(m_buffer, 0, _countof(m_buffer));
		};

		void Clear();
		void PushWord(uint32_t word);
	};

	const uint32_t REGION_MASK[8] = {
		//KUSEG: 2048MB
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
		//KSEG0 : 512 Mb
		0x7fffffff,
		//KSEG1: 512 Mb
		0x1fffffff,
		//KSEG2: 1024Mb
		0xffffffff, 0xffffffff
	};

	const _map CACHECONTROLL = _map(0xfffe0130, 4);
	const _map BIOS = _map(0x1fc00000, 512 * 1024);
	const _map MEMCONTROL = _map(0x1f801000, 36);
	const _map RAMSIZE = _map(0x1f801060, 4);
	const _map RAM = _map(0x00000000, 2 * 1024 * 1024);
	const _map SYSCONTROL = _map(0x1f801000, 36);
	const _map SPU = _map(0x1f801c00, 640);
	const _map EXPANSION2 = _map(0x1f802000, 66);
	const _map EXPANSION1 = _map(0x1f000000, 512 * 1024);
	const _map IRQCONTROL = _map(0x1f801070, 8);
	const _map TIMERS = _map(0x1f801100, 0x30);
	const _map DMA = _map(0x1f801080, 0x80);
	const _map GPU = _map(0x1f801810, 8);

	const uint32_t VERTEXBUFFERLEN = 64 * 1024;

	uint32_t mask_region(uint32_t addr);

	_port FromIndex(uint32_t index);
};


namespace pscx_rustf {
	bool CheckedSub(uint32_t what, uint32_t how);
	bool CheckedAdd(uint32_t what, uint32_t how);
	uint32_t WrappIntAdd(uint32_t pc, uint32_t incr);
	uint32_t WrappIntSub(uint32_t pc, uint32_t sub);
}