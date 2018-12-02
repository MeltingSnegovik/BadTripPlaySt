#pragma once

#include <vector>
#include "bios.h"
#include "map.h"
#include "ram.h"
#include "dma.h"
#include "gpu.h"

#define START_CASH_DATA 0xbfc00000


struct _interconnect {
public:
	
	_bios Bios;
	_ram ram;
	_dma d_DMA;
	pscx_gpu::_gpu d_gpu;
	_interconnect(_bios bios) :
		Bios(bios)
//		MemControll(0x1f801000,36)
	{
	};

	void Store32(uint32_t addr, uint32_t val);
	uint32_t Load32(uint32_t addr);
	void Store16(uint32_t addr, uint16_t val);
	void Store8(uint32_t addr, uint8_t val);
	uint8_t Load8(uint32_t addr);
	uint16_t Load16(uint32_t addr);
	//DMA register write
	void SetDmaReg(uint32_t offset, uint32_t val);
	uint32_t DmaReg(uint32_t offset);
	void DoDma(_port port);
	void DoDmaBlck(_port port);
	void DoDmaLinkedList(_port port);

};
	
