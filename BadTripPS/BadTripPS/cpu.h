#pragma once


#include "stdint.h"
#include <vector>
#include "instruction.h"
#include "interconnect.h"
#include "instruction.h"
#include "map.h"

#define BEGIN_BIOS 0xbfc00000

/*
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
*/

struct _cpu
{
	public:
		uint32_t pc;
		uint32_t next_pc;
		uint32_t current_pc;
		uint32_t regs_c[32];
		uint32_t hi;
		uint32_t lo;
		/*
		some non-standart definition
		tbd
		*/
		uint32_t StatReg; //cop0 StatusRegister
		bool branch;
		bool delay_slot;
		bool debugonbreak;
		_instruction instruction_c;
		_instruction next_instruction;
		_interconnect interconnect_c;
		
		_cpu(_interconnect inter):
			pc(BEGIN_BIOS),
			interconnect_c(inter),
			instruction_c(32),							// tbd
			next_instruction(0x0),
			StatReg(0)
			{
				memset(regs_c, 0xdeadbeef,sizeof(uint32_t)*32);
				regs_c[0]=0x0;
			};

		void SetDebugOnBreak(bool enable);
		void RunNextInstruction();
		uint32_t Load32(uint32_t addr);
		uint32_t WrappIntAdd(uint32_t pc, uint32_t incr);
		uint32_t WrappIntSub(uint32_t pc, uint32_t incr);
		int32_t CheckedAdd(uint32_t what, uint32_t how);
		void DecodeAndExecute(_instruction instruction);
		void OpLui(_instruction instruction);
		uint32_t Reg(uint32_t index);
		void SetReg(uint32_t index, uint32_t val);
		void OpOri(_instruction instruction);
		void Store32(uint32_t addr, uint32_t value);
		void OpSw(_instruction instruction);
		void OpSll(_instruction instruction);
		void OpAddiu(_instruction instruction);
		void OpJ(_instruction instruction);
		void OpOr(_instruction instruction);
		void OpCop0(_instruction instruction);
		void OpMtc0(_instruction instruction);
		void Branch(uint32_t offset);
		void OpBne(_instruction instruction);
		//Add Immediate Unsigned
		void OpAddi(_instruction instruction);

};
 
	
