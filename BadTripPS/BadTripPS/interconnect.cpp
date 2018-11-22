#include <vector>
#include <iostream>
#include "interconnect.h"
#include "map.h"

using namespace pscx_memory;

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

void _interconnect::Store16(uint32_t addr, uint16_t val) {
	if (addr % 2 != 0)
		std::cout << "Unaligned_store16_address: " << addr << std::endl;
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	if (SPU.contains(abs_addr) != (-1)) {
		std::cout << "Unaligned_write_to_SPU: " << SPU.contains(abs_addr) << std::endl;
		return;
	}
	std::cout << "Unhandled_store16_into_address: " << addr << std::endl;
};

void _interconnect::Store8(uint32_t addr, uint8_t val) {
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	uint32_t res = pscx_memory::EXPANSION2.contains(addr);
	if (res != (-1)) {
		std::cout << "Unhandled_store8_into_address: " << res << std::endl;
		return;
		}
	std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
};

uint8_t _interconnect::Load8(uint32_t addr) {
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	uint32_t ans = BIOS.contains(abs_addr);

	if (ans != (-1)) {
		//std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
		return Bios.Load8(ans);
	};

	uint32_t ans_exp = pscx_memory::EXPANSION1.contains(abs_addr);
	if (ans_exp != (-1)) {
		//std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
		return 0xff;
	};
	std::cout << "Unhandled_load8_into_address: " << addr << std::endl;
};
