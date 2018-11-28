#include <vector>
#include <iostream>
#include "interconnect.h"
#include "map.h"

using namespace pscx_memory;

void _interconnect::Store32(uint32_t addr, uint32_t value) {
	if (addr % 4 != 0)
		std::cout << "unhandled store32 into address " << addr << std::endl;
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	uint32_t RetMemContr = MEMCONTROL.contains(addr);
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

	uint32_t ans_irq_contrl = pscx_memory::IRQCONTROL.contains(abs_addr); //tbd
	if (ans_irq_contrl != (-1)) {
			std::cout << "IRQ_control: " << ans_irq_contrl <<" "<< value<< std::endl;
	};

	uint32_t dma_map_ret = pscx_memory::DMA.contains(abs_addr);
	if (dma_map_ret != (-1)) {
		std::cout << "dma_write: " << abs_addr << " " << value << std::endl;
		return;
	};

	uint32_t gpu_map_ret = pscx_memory::GPU.contains(abs_addr);
	if (gpu_map_ret != (-1)) {
		std::cout << "GPU_WRITE" << gpu_map_ret << " " << value << std::endl;
		return;
	};
};

uint32_t _interconnect::Load32(uint32_t addr) {
	uint32_t map_return = BIOS.contains(addr);
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	if (map_return != -1)
		return map_return;

	uint32_t irq_map_ret = pscx_memory::IRQCONTROL.contains(abs_addr);
	if (irq_map_ret != -1) {
		std::cout << "IRQ_control_read " << irq_map_ret << std::endl;
		return 0;
		}
	return -1;

	if (addr % 4 != 0)
		std::cout << "Unaligned_load32_address: {:08x}" << addr << std::endl;

	uint32_t dma_map_ret = pscx_memory::DMA.contains(abs_addr);
	if (dma_map_ret!=-1) {
		std::cout << "DMA read: " << abs_addr << std::endl;
		return 0;
	};

	uint32_t gpu_map_ret = pscx_memory::GPU.contains(abs_addr);
	if (gpu_map_ret != -1) {
		std::cout << "gpu read: " << gpu_map_ret << std::endl;
		switch (gpu_map_ret) {
		case 4:
			return 0x10000000;
		default:
			return 0;
		};
		return 0;
	};

	std::cout << "Unhandled_load32_address: {:08x}" << addr << std::endl;
};

void _interconnect::Store16(uint32_t addr, uint16_t val) {
	if (addr % 2 != 0)
		std::cout << "Unaligned_store16_address: " << addr << std::endl;
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	if (SPU.contains(abs_addr) != (-1)) {
		std::cout << "Unaligned_write_to_SPU: " << SPU.contains(abs_addr) << std::endl;
		return;
	}
	uint32_t timer_map_ret = pscx_memory::TIMERS.contains(abs_addr); //tbd
	if (SPU.contains(abs_addr) != (-1)) {
		std::cout << "Unhandled_write_to_timer_register" << timer_map_ret << std::endl;
		return;
	}
	uint32_t ram_mar_ret = pscx_memory::RAM.contains(abs_addr);
	if (ram_mar_ret != -1) {
		ram.Store16(ram_mar_ret, val);
		return;
	}
	
	uint32_t irq_mar_ret = pscx_memory::IRQCONTROL.contains(abs_addr);
	if (irq_mar_ret != -1) {
		std::cout << "IRQ controll write" << irq_mar_ret << " "<< val << std::endl;
		return;
	}

	uint32_t tm_mar_ret = pscx_memory::TIMERS.contains(abs_addr);
	if (tm_mar_ret != -1) {
		std::cout << "Unhandled write to timer" << tm_mar_ret << " " << val << std::endl;
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

	uint32_t ans_ram = pscx_memory::RAM.contains(abs_addr);
	if (res != (-1)) {
		std::cout << "Unhandled_store8_into_address: " << res << std::endl;
		ram.Store8(ans_ram, val);
		return;
	}
	std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
};

uint8_t _interconnect::Load8(uint32_t addr) {
	uint32_t abs_addr = pscx_memory::mask_region(addr);


	uint32_t ans_ram = pscx_memory::RAM.contains(abs_addr);

	if (ans_ram != (-1)) {
		return ram.Load8(ans_ram);
	};

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

uint16_t _interconnect::Load16(uint32_t addr) {
	uint32_t abs_addr = pscx_memory::mask_region(addr);

	uint32_t spu_map_ret = pscx_memory::SPU.contains(abs_addr);
	if (spu_map_ret != (-1)) {
		std::cout << "Unhandled read from spu: " << abs_addr << std::endl;
		return 0;
	};

	uint32_t ram_map_ret = pscx_memory::RAM.contains(abs_addr);
	if (ram_map_ret != (-1)) {
		return ram.Load16(ram_map_ret);
	};

	uint32_t irq_map_ret = pscx_memory::IRQCONTROL.contains(abs_addr);
	if (irq_map_ret != (-1)) {
		std::cout << "IRQ control" << irq_map_ret << std::endl;
		return 0;
	};
}