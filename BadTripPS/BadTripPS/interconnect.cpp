#include <vector>
#include <iostream>
#include "interconnect.h"
#include "map.h"

uint32_t _interconnect::load32(uint32_t addr) {

//	_map BIOS_Range(0xbfc00000,512*1024);
	uint32_t map_return = BIOS.contains(addr);
	if (map_return != -1)
		return map_return;

	return -1;
};

void _interconnect::Store32(uint32_t addr, uint32_t value) {
	if (addr % 4 != 0)
		std::cout << "unhandled store32 into address " <<addr<< std::endl;

	uint32_t RetMemContr = MemControll.contains(addr);
	if (RetMemContr = -1) {
		switch (RetMemContr) {
		case 0:
			if (value != 0x1f000000)
				std::cout << "Bad expansion 1 base address: 0x" << value << std::endl;
			break;
		case 4:
			if (value != 0x1f802000)
				std::cout << "Bad expansion 2 base address: 0x" << value << std::endl;
			break;
		default:
			std::cout << "Unhandled write to MEMcontroll register" << std::endl;
		}
	};

};

uint32_t _interconnect::Load32(uint32_t addr) {
	if (addr % 4 != 0)
		std::cout << "Unaligned_load32_address: {:08x}" << addr << std::endl;
};