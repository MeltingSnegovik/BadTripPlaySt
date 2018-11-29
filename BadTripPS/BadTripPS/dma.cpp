#include "dma.h"

uint32_t _dma::Control() {
	return d_control;
};

void _dma::SetControl(uint32_t val) {
	d_control = val;
};

bool _dma::Irq() {
	auto channel_irq = d_channel_irq_flg & d_channel_irq_en;
	return (d_force_irq || (d_irq_en && channel_irq != 0));
};

uint32_t _dma::Interrupt() {
	uint32_t r = 0;
	r |= (uint32_t)d_irq_dummy;
	r |= ((uint32_t)d_force_irq) << 15;
	r |= ((uint32_t)d_channel_irq_en) << 16;
	r |= ((uint32_t)d_irq_en) << 23;
	r |= ((uint32_t)d_channel_irq_flg) << 24;
	r |= ((uint32_t)Irq()) << 31;
	
	return r;
};

void _dma::SetInterrupt(uint32_t val) {
	d_irq_dummy = (uint8_t)(val & 0x3f);
	d_force_irq = ((val >> 15) & 1 != 0);
	d_channel_irq_en = (uint8_t)((val >> 16) & 0x7f);
	d_irq_en = ((val >> 24) & 1 )!= 0;
	uint8_t ack = ((val >> 24) & 0x3f);
	d_channel_irq_flg &= !ack;
};

_channel _dma::Channel(_port Port) {
	return d_Channels[Port];
};

