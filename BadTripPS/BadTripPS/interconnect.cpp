#include <vector>
#include <iostream>
#include "interconnect.h"
#include "map.h"
#include "channel.h"


using namespace pscx_memory;

void _interconnect::Store32(uint32_t addr, uint32_t value) {
	if (addr % 4 != 0)
		std::cout << "unhandled store32 into address " << addr << std::endl;
	uint32_t abs_addr = pscx_memory::mask_region(addr);
	uint32_t RetMemContr = pscx_memory::MEMCONTROL.contains(abs_addr);
	if (RetMemContr != -1) {
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
		SetDmaReg(dma_map_ret, value);
		std::cout << "dma_write: " << abs_addr << " " << value << std::endl;
		return;
	};

	uint32_t gpu_map_ret = pscx_memory::GPU.contains(abs_addr);
	if (gpu_map_ret != (-1)) {
		switch (gpu_map_ret) {
		case 0:
			d_gpu.Gp0(value);
			break;
		default:
			std::cout << "GPU write: " << gpu_map_ret << " " << value << std::endl;
			break;
		};
		return;
	};
};

uint32_t _interconnect::Load32(uint32_t addr) {
	
	uint32_t abs_addr = pscx_memory::mask_region(addr);

	if (abs_addr % 4 != 0) {
		std::cout << "Unaligned_load32_address: {:08x}" << abs_addr << std::endl;
		return 0;
	};

	uint32_t bios_map_return = BIOS.contains(abs_addr);
	if (bios_map_return != -1)
		return d_bios.Load32(bios_map_return);

	uint32_t ram_map_ret = RAM.contains(abs_addr);
	if (ram_map_ret != -1)
		return d_ram.Load32(ram_map_ret);


	uint32_t irq_map_ret = pscx_memory::IRQCONTROL.contains(abs_addr);
	if (irq_map_ret != -1) {
		std::cout << "IRQ_control_read " << irq_map_ret << std::endl;
		return 0;
		}


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
			return 0x1c000000;
			break;
		default:
			return 0;
			break;
		};
		return 0;
	};

	std::cout << "Unhandled_load32_address: {:08x}" << addr << std::endl;
	return -1;
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
		d_ram.Store16(ram_mar_ret, val);
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
	uint32_t res = pscx_memory::EXPANSION2.contains(abs_addr);
	if (res != (-1)) {
		std::cout << "Unhandled_store8_into_address: " << res << std::endl;
		return;
		}

	uint32_t ans_ram = pscx_memory::RAM.contains(abs_addr);
	if (res != (-1)) {
		std::cout << "Unhandled_store8_into_address: " << res << std::endl;
		d_ram.Store8(ans_ram, val);
		return;
	}
	std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
};

uint8_t _interconnect::Load8(uint32_t addr) {
	uint32_t abs_addr = pscx_memory::mask_region(addr);


	uint32_t ans_ram = pscx_memory::RAM.contains(abs_addr);
	if (ans_ram != (-1)) {
		return d_ram.Load8(ans_ram);
	};

	uint32_t ans = BIOS.contains(abs_addr);
	if (ans != (-1)) {
		//std::cout << "Unhandled_store8_into_address: " << addr << std::endl;
		return d_bios.Load8(ans);
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
		return d_ram.Load16(ram_map_ret);
	};

	uint32_t irq_map_ret = pscx_memory::IRQCONTROL.contains(abs_addr);
	if (irq_map_ret != (-1)) {
		std::cout << "IRQ control" << irq_map_ret << std::endl;
		return 0;
	};
};
/*
uint32_t _interconnect::DmaReg(uint32_t offset) {
	switch (offset) {
	case 0x70:
		d_DMA.Control();
		break;
	default:
		std::cout << "unhandled DMA access" << std::endl;
		break;
	};
};
*/

uint32_t _interconnect::DmaReg(uint32_t offset) {
	uint32_t major = (offset & 0x70) >> 4;
	uint32_t minor = (offset & 0xf);

	_channel channel;

	switch (major) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		channel = d_DMA.Channel(pscx_memory::FromIndex(major));
		switch (minor) {
		case 8:
			return channel.Control();
			break;
		default:
			std::cout << "Unhandled DMA read at: " << offset << std::endl;
			break;
		};
		break;
	case 7:
		switch (minor) {
		case 0:
			return d_DMA.Control();
			break;
		case 4:
			return d_DMA.Interrupt();
			break;
		default:
			std::cout << "Unhandled DMA read at: " << offset << std::endl;
			break;
		};
		break;
	default:
		std::cout << "Unhandled DMA read at: "<< offset << std::endl;
		break;
	};
	return 0;
};

void _interconnect::SetDmaReg(uint32_t offset, uint32_t val) {
	uint32_t major = (offset & 0x70) >> 4;
	uint32_t minor = (offset & 0xf);

	_channel c_channel;
	_port c_port;
	_port c_act_port;
	switch (major) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		c_port = pscx_memory::FromIndex(major);
		c_channel = d_DMA.Channel(c_port);
		switch (minor) {
		case 0:
			c_channel.SetBase(val);
			break;
		case 4:
			c_channel.SetBlCntr(val);
			break;
		case 8:
			c_channel.SetControl(val);
			break;
		default:
			std::cout << "Unhandled DMA write at: " << offset <<" "<< val << std::endl;
			break;
		};
		if (c_channel.Active()) {
			c_act_port = c_port;
		}
		else {
			
		};
		break;
	case 7:
		switch (minor) {
		case 0:
			d_DMA.SetControl(val);
			break;
		case 4:
			d_DMA.SetInterrupt(val);
			break;
		default:
			std::cout << "Unhandled DMA write at: " << offset << " " << val << std::endl;
			break;
		};
	default:
		std::cout << "Unhandled DMA write at: " << offset << " " << val << std::endl;
		break;
	};

	//tbd if let Some(port) = active_port
	if (c_act_port!=-1) {
		DoDma(c_port);
	}
};

void _interconnect::DoDmaBlck(_port port) {
	_channel c_channel = d_DMA.Channel(port);

	uint32_t c_incr;
	switch (c_channel.d_Step) {
	case _channel::_step::e_Incr:
		c_incr = 4;
		break;
	case _channel::_step::e_Dec:
		c_incr = -4;
		break;
	};

	uint32_t addr = c_channel.Base();
	uint32_t remsz;
	if (c_channel.TransferSize()!=-1) {
		remsz = c_channel.TransferSize();
	}
	else {
		std::cout << "couldn't figure out DMA block transfer size" << std::endl;
	};

	while (remsz > 0) {
		uint32_t cur_addr = addr & 0x1ffffc;
		uint32_t src_word;
		switch (c_channel.Direction()) {
		case _channel::_direction::e_FromRam:
			src_word= d_ram.Load32(cur_addr);
			switch (port) {
			case _port::e_Gpu:
				d_gpu.Gp0(src_word);
				break;
			default:
				std::cout << "Unhandled DMA dest port " << port << std::endl;
				break;
			};
			break;
		case _channel::_direction::e_ToRam:
			switch (port) {
			case _port::e_Otc:
				switch (remsz) {
				case 1:
					src_word = 0xffffff;
					break;
				default:
					src_word = pscx_rustf::WrappIntSub(addr, 4) & 0x1fffff;
					break;
				};
				break;
			default:
				d_ram.Store32(cur_addr, src_word);
				break;
			};
		};
		addr = pscx_rustf::WrappIntAdd(addr, c_incr);
		remsz--;
	};
	c_channel.Done();
};

//dma.channel.sync
void _interconnect::DoDma(_port port) {
	switch (d_DMA.Channel(port).d_Sync) {
	case _channel::_sync::e_LinkedList:
		DoDmaLinkedList(port);
		break;
	default:
		DoDmaBlck(port);
		break;
	};
};

void _interconnect::DoDmaLinkedList(_port port) {
	_channel c_channel = d_DMA.Channel(port);
	uint32_t addr = c_channel.Base() & 0x1ffffc;

	if (c_channel.Direction() == _channel::_direction::e_ToRam) {
		std::cout << "invalid DMA direction for linked list mode" << std::endl;
	};

	if (port != _port::e_Gpu) {
		std::cout << "attemp linked list dma on port" << std::endl;
	};

	while (true) {

		uint32_t header = d_ram.Load32(addr);
		uint32_t remsz = header >> 24;
		while (remsz > 0) {
			addr = (addr + 4) & 0x1ffffc;
			uint32_t command = d_ram.Load32(addr);
			d_gpu.Gp0(command);
			remsz--;
		};
	if (header & 0x800000 != 0){
		break;
		}
	addr = header & 0x800000;
	};
	c_channel.Done();
};




