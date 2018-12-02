#include "map.h"



uint32_t _map::contains(const uint32_t addr) {
	if (addr >= map_x1 && addr < map_x1 + map_x2)
		return (addr - map_x1);
	else
		return -1;
};


void _buffer::Clear() {
	m_len = 0;
};

void _buffer::PushWord(uint32_t word) {
	m_buffer[m_len] = word;
	m_len |= 1;
};

// tbd index pg 127


