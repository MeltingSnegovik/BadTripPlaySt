#include "channel.h"

uint32_t _channel::Control() {
	uint32_t r = 0;
	r |= ((uint32_t)d_Direct) << 0;
	r |= ((uint32_t)d_Step) << 1;
	r |= ((uint32_t)d_Chop) << 8;
	r |= ((uint32_t)d_Sync) << 9;
	r |= ((uint32_t)d_chop_dma_sz) << 16;
	r |= ((uint32_t)d_chop_cpu_sz) << 20;
	r |= ((uint32_t)d_enabl) << 24;
	r |= ((uint32_t)d_Trig) << 28;
	r |= ((uint32_t)d_dummy) << 29;

	return r;
};

void _channel::SetControl(uint32_t val) {
	switch (val & 1 != 0) {
	case true:
		d_Direct = _direction::e_FromRam;
		break;
	case false:
		d_Direct = _direction::e_ToRam;
		break;
	};

	switch ((val >> 1) & 1 != 0) {
	case true:
		d_Step = _step::e_Dec;
		break;
	case false:
		d_Step = _step::e_Incr;
		break;
	};

	d_Chop = (val >> 8) & 1 != 0;

	switch ((val >> 9) & 3) {
	case 0:
		d_Sync = _sync::e_Man;
		break;
	case 1:
		d_Sync = _sync::e_Request;
		break;
	case 2:
		d_Sync = _sync::e_LinkedList;
		break;
	default:
		std::cout << "Unknown DMA sync mode " << ((val >> 9) & 3) << std::endl;
		break;
	};

	d_chop_dma_sz = (uint8_t)((val >> 16) & 7);
	d_chop_cpu_sz = (uint8_t)((val >> 20) & 7);

	d_enabl = (val >> 24) & 1 != 0;
	d_Trig = (val >> 28) & 1 != 0;

	d_dummy = (uint8_t)((val >> 29) & 3);
};


uint32_t _channel::Base() {
	return d_base;
};

void _channel::SetBase(uint32_t val) {
	d_base = val & 0xffffff;
};

uint32_t _channel::BlCntr() {
	return (((uint32_t)d_bl_size << 16) | (uint32_t)d_bl_cnt);
};
void _channel::SetBlCntr(uint32_t val) {
	d_bl_size = (uint16_t)val;
	d_bl_cnt = (uint16_t)(val >> 16);
};

bool _channel::Active() {
	bool c_trig;
	switch (d_Sync) {
	case _sync::e_Man:
		c_trig = d_Trig;
		break;
	default:
		c_trig = true;
		break;
	}
	return (d_enabl&&c_trig);
};

_channel::_direction _channel::Direction() {
	return d_Direct;
};

_channel::_step _channel::Step() {
	return d_Step;
};

_channel::_sync _channel::Sync() {
	return d_Sync;
};

uint32_t _channel::TransferSize() {
	uint32_t bs = (uint32_t)d_bl_size;
	uint32_t bc = (uint32_t)d_bl_cnt;

	switch (Sync()) {
	case _sync::e_Man:
		return bs;
		break;
	case _sync::e_Request:
		return bs*bc;
		break;
	case _sync::e_LinkedList:
		return 0;
		break;
	}
};

void _channel::Done() {
	d_enabl = false;
	d_Trig = false;
}