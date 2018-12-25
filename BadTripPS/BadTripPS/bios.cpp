#pragma once

#include "bios.h"


//_bios::BiosState _bios::LoadBios(std::string path_c) {
_bios::BiosState _bios::LoadBios() {
		std::basic_ifstream<uint8_t> biosFile("D:\\cpp\\BadTripPlaySt\\BadTripPS\\BadTripPS\\SCPH1001.BIN", std::ios::in | std::ios::binary); // char?

		if (!biosFile.good())
			return BIOS_STATE_INCORRECT_FILENAME;
		const uint32_t biosSize = 512 * 1024; // 512 kb
											  // Load the BIOS
		data.insert(data.begin(), std::istreambuf_iterator<uint8_t>(biosFile), std::istreambuf_iterator<uint8_t>());
		biosFile.close();
		if (data.size() != biosSize)
			return BIOS_STATE_INVALID_BIOS_SIZE;
	};

uint32_t _bios::Load32(uint32_t offset) {
	uint32_t b0=data[offset+0];
	uint32_t b1=data[offset+1];
	uint32_t b2=data[offset+2];
	uint32_t b3=data[offset+3];

	return b0 | (b1 << 8) | (b2 << 16) | (b3<<24);
};

uint16_t _bios::Load16(uint32_t offset) {
	uint32_t b0 = data[offset + 0];
	uint32_t b1 = data[offset + 1];

	return b0 | (b1 << 8);
};

uint8_t _bios::Load8(uint32_t offset) {
	return data[offset];
};





	
