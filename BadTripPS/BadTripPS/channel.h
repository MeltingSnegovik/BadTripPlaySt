#pragma once
#include <cstdint>

struct _channel {

	enum _direction {
		e_ToRam = 0,
		e_FromRam =1
	};
	
	enum _step {
		e_Incr =0,
		e_Dec = 1
	};

	enum _sync {
		e_Man = 0,
		e_Request= 1,
		e_LinkedList=2
	};

	bool d_enabl;
	_direction d_Direct;
	_step d_Step;
	_sync d_Sync;
	bool d_Trig;
	bool d_Chop;
	uint8_t d_chop_dma_sz;
	uint8_t d_chop_cpu_sz;
	uint8_t d_dummy;
	uint32_t d_base;
	uint16_t d_bl_size;
	uint16_t d_bl_cnt;

	_channel() :
		d_enabl(false),
		d_Direct(_direction::e_ToRam),
		d_Step(_step::e_Incr),
		d_Sync(_sync::e_Man),
		d_Trig(false),
		d_Chop(false),
		d_chop_dma_sz(0),
		d_chop_cpu_sz(0),
		d_dummy(0),
		d_base(0),
		d_bl_size(0),
		d_bl_cnt(0)
	{
	};

	uint32_t Control();
	void SetControl(uint32_t val);
	uint32_t Base();
	void SetBase(uint32_t val);
	uint32_t BlCntr();
	void SetBlCntr(uint32_t val);
	bool Active();
	_direction Direction();
	_step Step();
	_sync Sync();
	uint32_t TransferSize();
	void Done();
};


