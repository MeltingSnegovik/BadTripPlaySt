#include "map.h"



uint32_t _map::contains(uint32_t addr) {
	if (addr >= map_x1 && addr < map_x1 + map_x2)
		return (addr - map_x1);
	else
		return -1;
} 
