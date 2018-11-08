#pragma once


#include "stdint.h"
#include <vector>
#include "instruction.h"
#include "interconnect.h"

#define BEGIN_BIOS 0xbfc00000


struct _map
{
public:
	uint32_t start_data;
	uint32_t end_data;
	uint32_t contains(uint32_t addr);
	_map() :
		start_data(0xbfc00000),
		end_data(512 * 1024)
	{};

};

struct _cpu
{
	public:
		uint32_t pc;
		uint32_t next_pc;
		uint32_t current_pc;
//		uint32_t[32] regs_c;
		uint32_t hi;
		uint32_t lo;
		/*
		some non-standart definition
		tbd
		*/
		bool branch;
		bool delay_slot;
		bool debugonbreak;
		_instruction instruction_c;
		_interconnect interconnect_c;
		
		_cpu(_interconnect inter):
			pc(BEGIN_BIOS),
			interconnect_c(inter)
			{
				memset(regs_c, 0xdeadbeef,sizeof(uint32_t)*32);
				regs_c[0]=0x0;
			};
		void SetDebugOnBreak(bool enable);
		void RunNextInstruction();
		uint32_t WrappIntAdd(uint32_t pc, uint32_t incr);
		uint32_t DecodeAndExecute(_instruction instruction);		
};

	
