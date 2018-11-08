#pragma once

#include <vector>

struct _bios {
public:
	enum BiosState
	{
		BIOS_STATE_SUCCESS,
		BIOS_STATE_INCORRECT_FILENAME,
		BIOS_STATE_INVALID_BIOS_SIZE,
		BIOS_STATE_COUNT
	};

	BiosState load_bios();
	std::vector<uint8_t> data;

	uint32_t Load32(uint32_t offset);
	void NewPath();
	_bios();
};