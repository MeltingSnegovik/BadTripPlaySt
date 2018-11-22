#pragma once

#include <vector>
#include <string>

struct _bios {
public:
	
	std::string path;
	enum BiosState
	{
		BIOS_STATE_SUCCESS,
		BIOS_STATE_INCORRECT_FILENAME,
		BIOS_STATE_INVALID_BIOS_SIZE,
		BIOS_STATE_COUNT
	};

	BiosState LoadBios(std::string path_c);
	std::vector<uint8_t> data;

	uint32_t Load32(uint32_t offset);
	uint8_t Load8(uint32_t offset);
//	void NewPath();
/*	_bios(std::string path_c):
		path(path_c)
	{
	}; */
};