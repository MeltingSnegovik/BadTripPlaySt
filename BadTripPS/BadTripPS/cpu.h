#pragma once


#include "stdint.h"
#include <vector>
#include "instruction.h"
#include "interconnect.h"
#include "instruction.h"
#include "map.h"
#include <cassert>
#include <climits>
#include <iostream>
#include <fstream>
#include <string>


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
		pscx_memory::_regIndex	d_regIndex;
		uint32_t	d_regValue;
		_registerData(pscx_memory::_regIndex regInd, uint32_t regVal) :
			d_regIndex(regInd),
			d_regValue(regVal)
		{};
	};
	public:
		enum _exception {
			e_SYSCALL = 0x8,
			e_OVERFLOW= 0xc,
			e_LOADADDRESSERROR = 0x4,
			e_STOREADDRESSERROR = 0x5,
			e_BREAK = 0x9,
			e_COPROCESSORERROR = 0xb,
			e_ILLEGALINSTRUCTION = 0xa
		};


		uint32_t init_pc;
		uint32_t pc;
		uint32_t next_pc;

		uint32_t current_pc;

		uint32_t d_cause;
		uint32_t d_epc;

		uint32_t regs_c[32];
//		uint32_t out_regs[32];

		uint32_t d_hi;
		uint32_t d_lo;
		/*
		some non-standart definition
		tbd
		*/

		// sr cop0 StatusRegister
		uint32_t StatReg; 

		bool d_branch;
		bool d_delay_slot;
		bool debugonbreak;

		_instruction instruction_c;
		_instruction next_instruction;
		_interconnect interconnect_c;
		//like a load in CPU
		_registerData d_regData;
		std::ofstream file_out;
		std::ifstream golden_res;
		
		_cpu(_interconnect inter) :
			pc(BEGIN_BIOS),
			interconnect_c(inter),
			next_pc(0),
			instruction_c(0),// tbd
			current_pc(0),
			//	pc(BEGIN_BIOS),
			next_instruction(0x0),
			StatReg(0),
			d_regData(pscx_memory::_regIndex(0x0),0x0),
			d_hi(0xdeadbeef),
			d_lo(0xdeadbeef),
			d_branch(false),
			d_delay_slot(false)
			{
				file_out = std::ofstream("D:\\cpp\\BadTripPlaySt\\output.txt");
				golden_res = std::ifstream("D:\\cpp\\BadTripPlaySt\\golden_result.txt");
				current_pc = pc;
				next_pc = pscx_rustf::WrappIntAdd(pc, 4);
				memset(regs_c, 0xdeadbeef,sizeof(uint32_t)*32);
				regs_c[0]=0x0;
				next_pc = pscx_rustf::WrappIntAdd(BEGIN_BIOS, 4);
				instruction_c = _cpu::Load32(BEGIN_BIOS);
				next_instruction = instruction_c;
				d_regData = _registerData(pscx_memory::_regIndex(0x0), 0x0);
			};

		void SetDebugOnBreak(bool enable);
		void RunNextInstruction();
		void DecodeAndExecute(_instruction instruction);
		void OpLui(_instruction instruction);
		uint32_t Reg(pscx_memory::_regIndex index);
		void SetReg(pscx_memory::_regIndex index, uint32_t val);
		void SetPC(uint32_t f_pc);
		void OpOri(_instruction instruction);

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
		
		_instruction Load32(uint32_t addr);
		_instruction Load16(uint32_t addr);
		_instruction Load8(uint32_t addr);
		
		void Store32(uint32_t addr, uint32_t value);
		void Store16(uint32_t addr, uint16_t val);
		void Store8(uint32_t addr, uint8_t val);

		void OpSh(_instruction instruction);
		void OpSb(_instruction instruction);
		void OpJal(_instruction instruction);
		void OpAndi(_instruction instruction);
		void OpJr(_instruction instruction);



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
		// Move from HI
		void OpMfhi(_instruction instruction);
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

		//Store 8 tbd
		//load 16


		// Load Halfword Unsigned
		void OpLhu(_instruction instruction);
		// Shift Left Logical Variable
		void OpSllv(_instruction instruction);

		// load Hlfword signed
		void OpLh(_instruction instruction);

		// Bitwise Not Or
		void OpNor(_instruction instruction);
		
		//Shift Right arithmetic variable 
		void OpSrav(_instruction instruction);

		//Shift Reight Logical Variable
		void OpSrlv(_instruction instruction);

		// Multiply Unsigned
		void OpMultu(_instruction instruction);

		// Bitwise Exclusive Or
		void OpXor(_instruction instruction);

		// Break
		void OpBreak(_instruction instruction);
								  
		//Multiply signed
		void OpMult(_instruction instruction);

		//Substract and check for signed overflow
		void OpSub(_instruction instruction);

		//Bitwise Exckusive or immediate
		void OpXori(_instruction instruction);

		// Coprocessor 1 opcode
		void OpCop1(_instruction instruction);

		// Coprocessor 2 opcode
		void OpCop2(_instruction instruction);
		
		// Coprocessor 3 opcode
		void OpCop3(_instruction instruction);

		// Load word left
		void OpLwl(_instruction instruction);

		// Load word right
		void OpLwr(_instruction instruction);

		//Store Word Left
		void OpSwl(_instruction instruction);

		//Store Word Right
		void OpSwr(_instruction instruction);

		//Load Word in Coprocessor 0
		void OpLwc0(_instruction instruction);

		//Load Word in Coprocessor 1
		void OpLwc1(_instruction instruction);

		//Load Word in Coprocessor 2
		void OpLwc2(_instruction instruction);

		//Load Word in Coprocessor 3
		void OpLwc3(_instruction instruction);

		//Store word in Coprocessor 0
		void OpSwc0(_instruction instruction);

		//Store word in Coprocessor 1
		void OpSwc1(_instruction instruction);

		//Store word in Coprocessor 2
		void OpSwc2(_instruction instruction);

		//Store word in Coprocessor 3
		void OpSwc3(_instruction instruction);

		//Illegal instruction
		void OpIllegal(_instruction instruction);

		void DelayedLoad();
};
 

