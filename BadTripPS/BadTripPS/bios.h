#pragma once

#include <vector>

struct _bios {

	_bios();
	std::vector<uint8_t> data;

	uint32_t Load32(uint32_t offset);
	void NewPath();
}