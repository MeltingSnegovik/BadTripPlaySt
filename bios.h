#pragma once

#include <vector>

#define
struct _bios {
	std::vector<uint8_t> data
	
	_bios():{
		std::basic_ifstream<uint8_t> biosFile(path, std::ios::in | std::ios::binary);

    	if (!biosFile.good())
			return BIOS_STATE_INCORRECT_FILENAME;
    	const uint32_t biosSize = 512 * 1024; // 512 kb
   		// Load the BIOS
		m_data.insert(m_data.begin(), std::istreambuf_iterator<uint8_t>(biosFile), std::istreambuf_iterator<uint8_t>());
    	biosFile.close();
		if (m_data.size() != biosSize)
        			return BIOS_STATE_INVALID_BIOS_SIZE;
		}

	uint32_t Load32(uint32_t offset);
	void NewPath()	
}
	
