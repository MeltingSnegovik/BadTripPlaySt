#pragma once


#include "stdint.h"
#include <vector>
#include "instruction.h"
#include "interconnect.h"
#include "instruction.h"
#include "map.h"
#include <cassert>
#include <climits>

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
	private:

	
	struct _registerData
	{
		_regIndex	d_regIndex;
		uint32_t	d_regValue;
		_registerData(_regIndex regInd, uint32_t regVal) :
			d_regIndex(regInd),
			d_regValue(regVal)
		{};
	};
	public:
		enum _exception {
			SYSCALL = 0x8
		};


		uint32_t init_pc;
		uint32_t pc;
		uint32_t next_pc;

		uint32_t current_pc;

		uint32_t d_cause;
		uint32_t d_epc;

		uint32_t regs_c[32];
		uint32_t out_regs[32];

		uint32_t d_hi;
		uint32_t d_lo;
		/*
		some non-standart definition
		tbd
		*/

		// sr cop0 StatusRegister
		uint32_t StatReg; 
		bool branch;
		bool delay_slot;
		bool debugonbreak;
		_instruction instruction_c;
		_instruction next_instruction;
		_interconnect interconnect_c;
		//like a load in CPU
		_registerData d_regData;
		
		_cpu(_interconnect inter) :
			init_pc(BEGIN_BIOS),
			instruction_c(_instruction(Load32(pc))),// tbd
			current_pc(pc),
//	pc(BEGIN_BIOS),
			next_pc(WrappIntAdd(init_pc,4)),
			interconnect_c(inter),
			next_instruction(0x0),
			StatReg(0),
			d_regData(_regIndex(0x0),0x0),
			d_hi(0xdeadbeef),
			d_lo(0xdeadbeef)
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
		uint32_t Reg(_regIndex index);
		void SetReg(_regIndex index, uint32_t val);
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
		void OpAddi(_instruction instruction); // Add integer 4+-1 good 0xfffffff+4 is overflow
		void OpLw(_instruction instruction); //Load Word
		//Set on Less Then Unsigned
		void OpSltu(_instruction instruction);
		void OpAddu(_instruction instruction);

		void Store16(uint32_t addr, uint16_t val);

		void OpSh(_instruction instruction);
		void OpJal(_instruction instruction);
		void OpAndi(_instruction instruction);
		void OpJr(_instruction instruction);

		uint8_t Load8(uint32_t ddr);

		//load byte (signed)
		void OpLb(_instruction instruction);
		// Branch if equal
		void OpBeq(_instruction instruction);
		// Move From Coprocessor 0
		void OpMfc0(_instruction instruction);
		// Bitwise And
		void OpAnd(_instruction instruction);
		// Add and generate an exception on overflowing
		void OpAdd(_instruction instruction);
		//Branch if Great Rhen Zero
		void OpBgtz(_instruction instruction);
		//Branch if Less than or Equal to Zero 
		void OpBlez(_instruction instruction);
		//Load Byte Unsigned
		void OpLbu(_instruction instruction);
		//Jump And Link Register
		void OpJalr(_instruction instruction);
		//various branch instruction BGEZ, BLTX, BGEZAL, BLTZAL
		void OpBxx(_instruction instruction);
		//Set if Less Than Immediate (signes)
		void OpSlti(_instruction instruction);
		//Substract Unsigned
		void OpSubu(_instruction instruction);
		//Shift Right Arithmetic
		void OpSra(_instruction instruction);
		// Divide
		void OpDiv(_instruction instruction);
		// Move from LO
		void OpMflo(_instruction instruction);
		// shift Right 
		void OpSrl(_instruction instruction);
		//Set if less than Immediate Unsigned
		void OpSltiu(_instruction instruction);
		//Divide Unsigned
		void OpDivu(_instruction instruction);
		// Move from Hi
		void OpMvhi(_instruction instruction);
		// Set on Less Than
		void OpSlt(_instruction instruction);
		// Trigger as exception
		void Exception(_exception cause);
		//System Call
		void OpSysCall(_instruction instruction);
		//Move to LO
		void OpMtlo(_instruction instruction);
		//Move to HI
		void OpMthi(_instruction instruction);
		//Return from Exception
		void OpRfe(_instruction instruction);
};
 

