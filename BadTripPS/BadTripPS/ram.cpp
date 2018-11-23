#include "ram.h"

uint32_t _ram::Load32(uint32_t offset) {
	uint32_t b0 = (uint32_t)m_data[offset + 0];
	uint32_t b1 = (uint32_t)m_data[offset + 1];
	uint32_t b2 = (uint32_t)m_data[offset + 2];
	uint32_t b3 = (uint32_t)m_data[offset + 3];

	return (b0 | b1 << 8 | b2 << 16 | b3 << 24);
};

void _ram::Store32(uint32_t offset, uint32_t val) {
	uint8_t b0 = val;
	uint8_t b1 = val>>8;
	uint8_t b2 = val>>16;
	uint8_t b3 = val>>24;

	m_data[offset + 0] = b0;
	m_data[offset + 1] = b1;
	m_data[offset + 2] = b2;
	m_data[offset + 3] = b3;
};

void _ram::Store8(uint32_t offset, uint8_t val) {
	m_data[offset] = val;
};

uint8_t _ram::Load8(uint32_t offset) {
	return m_data[offset];
};

