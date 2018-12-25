#pragma once

#include <vector>
#include <iostream>
#include "ram.h"
#include "dma.h"
#include "gpu.h"
#include "wind.h"
#include "map.h"
#include "channel.h"
#include "instruction.h"

#define START_CASH_DATA 0xbfc00000


struct _interconnect {
	_bios d_bios;
	_ram d_ram;
	_dma d_DMA;
	pscx_wind::_wind d_wind;
	int a;
	pscx_gpu::_gpu d_gpu;
	_interconnect(_bios bios,pscx_wind::_wind wind) :
		a(500),
		d_bios(bios),
		d_wind(wind),
		d_gpu(wind)
//		MemControll(0x1f801000,36)
	{
	};

	void Store32(uint32_t addr, uint32_t val);
	void Store16(uint32_t addr, uint16_t val);
	void Store8(uint32_t addr, uint8_t val);
	_instruction Load32(uint32_t addr);
	_instruction Load16(uint32_t addr);
	_instruction Load8(uint32_t addr);

	//DMA register write
	void SetDmaReg(uint32_t offset, uint32_t val);
	uint32_t DmaReg(uint32_t offset);
	void DoDma(pscx_memory::_port port);
	void DoDmaBlck(pscx_memory::_port port);
	void DoDmaLinkedList(pscx_memory::_port port);

};
	
