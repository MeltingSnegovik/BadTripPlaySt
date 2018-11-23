#pragma once

#include <vector>

struct _ram {
	std::vector<uint8_t> m_data;

	_ram() {
		m_data.resize(2 * 1024 * 1024);
		memset(m_data.data(), 0xca, m_data.size());
	}

	uint32_t Load32(uint32_t offset);
	void Store32(uint32_t offset, uint32_t val);
	void Store8(uint32_t offset, uint8_t val);
	uint8_t Load8(uint32_t offset);
};