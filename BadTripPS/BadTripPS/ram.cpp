#include "ram.h"

uint32_t _ram::Load32(uint32_t offset) {
	//
	uint32_t newOffset = offset & 0x1fffff;
	uint32_t b0 = (uint32_t)m_data[newOffset + 0];
	uint32_t b1 = (uint32_t)m_data[newOffset + 1];
	uint32_t b2 = (uint32_t)m_data[newOffset + 2];
	uint32_t b3 = (uint32_t)m_data[newOffset + 3];

	return (b0 | b1 << 8 | b2 << 16 | b3 << 24);
};

void _ram::Store32(uint32_t offset, uint32_t val) {
	//
	uint32_t newOffset = offset & 0x1fffff;

	uint8_t b0 = val;
	uint8_t b1 = val>>8;
	uint8_t b2 = val>>16;
	uint8_t b3 = val>>24;

	m_data[newOffset + 0] = b0;
	m_data[newOffset + 1] = b1;
	m_data[newOffset + 2] = b2;
	m_data[newOffset + 3] = b3;
};

void _ram::Store8(uint32_t offset, uint8_t val) {
	//
	uint32_t newOffset = offset & 0x1fffff;

	m_data[newOffset] = val;
};

uint8_t _ram::Load8(uint32_t offset) {
	//
	uint32_t newOffset = offset & 0x1fffff;

	return (uint8_t)m_data[newOffset];
};

void _ram::Store16(uint32_t offset, uint16_t val) {
	//
	uint32_t newOffset = offset & 0x1fffff;

	uint8_t b0 = val;
	uint8_t b1 = (val >> 8);

	m_data[newOffset + 0] = b0;
	m_data[newOffset + 1] = b1;
};

uint16_t _ram::Load16(uint32_t offset) {
	//
	uint32_t newOffset = offset & 0x1fffff;

	uint16_t b0 = (uint16_t)m_data[newOffset + 0];
	uint16_t b1 = (uint16_t)m_data[newOffset + 1];
	return (uint16_t)b0 | (b1 << 8);
};