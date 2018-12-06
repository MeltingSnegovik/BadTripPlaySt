#pragma once

#include <vector>
#include "ram.h"
#include "dma.h"
#include "gpu.h"

#define START_CASH_DATA 0xbfc00000


struct _interconnect {
	_bios d_bios;
	_ram d_ram;
	_dma d_DMA;
	int a;
	pscx_gpu::_gpu d_gpu;
	_interconnect(_bios bios) :
		a(500),
		d_bios(bios)
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
	void DoDma(pscx_memory::_port port);
	void DoDmaBlck(pscx_memory::_port port);
	void DoDmaLinkedList(pscx_memory::_port port);

};
	
