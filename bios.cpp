#pragma once

#include <vector>

#include "bios.h"
uint32_t _bios::Load32(uint32_t offset) {
		uint32_t b0=data[offset+0];
		uint32_t b1=data[offset+1];
		uint32_t b2=data[offset+2];
		uint32_t b3=data[offset+3];

		return b0 | (b1 << 8) | (b2 << 16) | (b3<<24);
}



	
