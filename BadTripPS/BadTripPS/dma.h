#pragma once
#include "map.h"
#include "channel.h"
#include <iostream>

struct _dma 
{
	uint32_t d_control;
	bool d_irq_en;
	uint8_t d_channel_irq_en;
	uint8_t d_channel_irq_flg;
	bool d_force_irq;
	uint8_t d_irq_dummy;
	_channel d_Channels[7];


	_dma() :
		d_control(0x07654321)
	{};

	uint32_t Control();
	void SetControl(uint32_t val);
	bool Irq();
	uint32_t Interrupt();
	void SetInterrupt(uint32_t val);
	_channel Channel(_port Port);
};