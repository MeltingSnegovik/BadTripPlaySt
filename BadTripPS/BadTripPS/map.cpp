#include "map.h"



uint32_t pscx_memory::_map::contains(uint32_t addr) const{
	if (addr >= map_x1 && addr < map_x1 + map_x2)
		return (addr - map_x1);
	else
		return -1;
};

uint32_t pscx_memory::mask_region(uint32_t addr) {
	size_t index = addr >> 29;
	return addr & REGION_MASK[index];
};

pscx_memory::_port pscx_memory::FromIndex(uint32_t index) {
	switch (index) {
	case 0:
		return _port::e_MdecIn;
		break;
	case 1:
		return _port::e_MdecOut;
		break;
	case 2:
		return _port::e_Gpu;
		break;
	case 3:
		return _port::e_CdRom;
		break;
	case 4:
		return _port::e_Spu;
		break;
	case 5:
		return _port::e_Pio;
		break;
	case 6:
		return _port::e_Otc;
		break;
	default:
		std::cout << "invalid Port " << index << std::endl;
	};
};

void pscx_memory::_buffer::Clear() {
	m_len = 0;
};

void pscx_memory::_buffer::PushWord(uint32_t word) {
	m_buffer[m_len] = word;
	m_len |= 1;
};

bool pscx_rustf::CheckedSub(int32_t what, int32_t how) {
	if (((int64_t)what - (int64_t)how) <= std::numeric_limits<int32_t>::max() && ((int64_t)what - (int64_t)how) >= std::numeric_limits<int32_t>::min())
		return true;
	else
		return false;

};

bool pscx_rustf::CheckedAdd(int32_t what, int32_t how) {
	if (((int64_t)what + (int64_t)how) <= std::numeric_limits<int32_t>::max() && ((int64_t)what + (int64_t)how) >= std::numeric_limits<int32_t>::min())
		return true;
	else
		return false;
};

uint32_t pscx_rustf::WrappIntAdd(uint32_t pc, uint32_t incr) {
	return (uint32_t)(pc + incr);
};

uint32_t pscx_rustf::WrappIntSub(uint32_t pc, uint32_t sub) {
	return (uint32_t)(pc - sub);
};

uint32_t pscx_rustf::Negative(int32_t val) {
	return (std::numeric_limits<int32_t>::max() - val);
}

// tbd index pg 127


