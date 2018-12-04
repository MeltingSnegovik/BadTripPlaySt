#pragma once

#include "bios.h"
#include <iostream>
#include <gl\GL.h>

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
	0xfffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	//KSEG0 : 512 Mb
	0x7ffffff,
	//KSEG1: 512 Mb
	0x1ffffff,
	//KSEG2: 1024Mb
	0xfffffff, 0xfffffff 
};

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
	_map TIMERS(0x1f801100, 0x30);
	_map DMA(0x1f801080, 0x80);

	uint32_t mask_region(uint32_t addr) {
		size_t index = addr >> 29;
		return addr & REGION_MASK[index];
	};

	_port FromIndex(uint32_t index) {
		switch (index) {
		case 0:
			return _port::e_MdecIn;
			break;
		case 1:
			return _port::e_MdecOut;
			break;
		case 2:
			return _port::e_Gpu;
			break;
		case 3:
			return _port::e_CdRom;
			break;
		case 4:
			return _port::e_Spu;
			break;
		case 5:
			return _port::e_Pio;
			break;
		case 6:
			return _port::e_Otc;
			break;
		default:
			std::cout << "invalid Port " << index << std::endl;
		};
	};
};

