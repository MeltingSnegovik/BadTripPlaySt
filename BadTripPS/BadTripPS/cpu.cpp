#include <iostream>
#include <vector>

#include "cpu.h"


uint32_t _cpu:: WrappIntAdd(uint32_t pc, uint32_t incr) {
	if ((pc + incr) > 0xffffffff)
		return (uint32_t)(pc + incr);
	else
		return 0x00000000;
};

uint32_t _cpu::WrappIntSub(uint32_t pc, uint32_t sub) {
	if ((pc - sub) < 0x00000000)
		return (uint32_t)(pc - sub);
	else
		return 0x00000000;
};

int32_t _cpu::CheckedAdd(uint32_t what, uint32_t add) {
	if ((uint64_t)what + (uint64_t)add <=0xFFFFFFFF)
		return (int32_t)(what + add);
	else
		return 0;
};

void _cpu::SetDebugOnBreak(bool enable) {
	debugonbreak=enable;
};

uint32_t _cpu::Load32(uint32_t addr) {
	return interconnect_c.load32(addr);
};

void _cpu::RunNextInstruction() {
	instruction_c= next_instruction;
	_instruction new_instruction(Load32(pc));
	next_instruction = new_instruction;
	pc= WrappIntAdd(pc,4);
	SetReg(d_regData.d_regIndex, d_regData.d_regValue);
	this->DecodeAndExecute(instruction_c);

	//in rust regs_c = out_regs;
	std::copy(std::begin(out_regs), std::end(out_regs), std::begin(regs_c));
};


void _cpu::DecodeAndExecute(_instruction instruction) {
	std::cout << "Unhandled_instruction{:08x}" << std::endl;
	uint32_t retInstrFunct = instruction.Function();
	switch (retInstrFunct) {
		case 0b000000:
			switch (instruction.SubFunction()) {
				case 0b000000:
					OpSll(instruction);
					break;
				default:
					std::cout << "Unhandled instruction" << std::endl;
					break;
			};
			break;
		case 0b001111:
			OpLui(instruction);
			break;
		case 0b001101:
			OpOri(instruction);
			break;
		case 0b101011:
			OpSw(instruction);
			break;
		case 0b010000:
			OpCop0(instruction);
			break;
		default:
			std::cout << "error on DecodeAndExecute" << std::endl;
			break;
	};
	// else return error
};

void _cpu::OpLui(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	_regIndex t = instruction.RegIndex();
	uint32_t v = i << 16;

	this->SetReg(t, v);
};

void _cpu::OpOri(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	_regIndex t = instruction.RegIndex();
	uint32_t s = instruction.s(); // tbd

	uint32_t v = this->Reg(s);
	this->SetReg(t, v);
};

//Store Word
void _cpu::OpSw(_instruction instruction) {
	if (StatReg & 0x1000 != 0) {
		std::cout << "error in OpSw" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	uint32_t s = instruction.s(); //tbd
	
	uint32_t addr = WrappIntAdd(Reg(s),i);
	uint32_t v = Reg(t);
	return Store32(addr, v);
};

uint32_t _cpu::Reg(_regIndex index) {
	assert(index.m_index < _countof(regs_c), "Reg Overflow");
	return regs_c[index.m_index];
};

void _cpu::SetReg(_regIndex index, uint32_t value) {
	assert(index.m_index < _countof(regs_c), "Set Reg Overflow");
	regs_c[index.m_index] = value;
	regs_c[0] = 0;
};

void _cpu::Store32(uint32_t addr, uint32_t value) {
	interconnect_c.Store32(addr, value);
};

void _cpu::OpSll(_instruction instruction) {
	uint32_t i = instruction.Shift();
	_regIndex t = instruction.RegIndex();
	_regIndex d = instruction.RegInd15();

	uint32_t v = Reg(t) << i;
	SetReg(d, v);
};

void _cpu::OpAddiu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	uint32_t s = instruction.s();

	uint32_t v = WrappIntAdd(Reg(s), i);
	SetReg(t, v);
};

void _cpu::OpJ(_instruction instruction) {
	uint32_t i = instruction.ImmJump();
	pc = ((pc & 0xf0000000) | (i << 2));
};

void _cpu::OpOr(_instruction instruction) {
	/*
	uint32_t d = instruction.RegInd15();
	uint32_t s = instruction.s();
	uint32_t t = instruction.RegIndex();
	SetReg(d, v);
	uint32_t v = Reg(s) | Reg(t);
	*/
	SetReg(instruction.RegInd15(), Reg(instruction.s()) | Reg(instruction.RegIndex()))
};

void _cpu::OpCop0(_instruction instruction) {
	switch (instruction.CopOpCode()) {
	case 0b00100:
		this->OpMtc0(instruction);
		break;
	default:
		std::cout << "error in OpCop0" << std::endl;
		break;
	};
};

void _cpu::OpMtc0(_instruction instruction) {
	_regIndex cpu_r = instruction.RegIndex();
	_regIndex cop_r = instruction.RegInd15();

	uint32_t v = this->Reg(cpu_r);
	uint32_t answ;
	switch (cop_r.m_index) {
	case 3:
	case 5:
	case 6:
	case 7:
	case 9:
	case 11:
		if (v != 0)
			std::cout << "Error in OpMtc0 cop_r" << std::endl;
		break;
	case 12:
		StatReg = v;
		break;
	case 13:
		if (v != 0)
			std::cout << "Error in OpMtc0 V" << std::endl;
		break;
	default:
		std::cout << "error in OpCop0 register" << cop_r.m_index << std::endl;
		break;
	};
};


void _cpu::Branch(uint32_t offset) {
	offset = offset << 2;
	pc = WrappIntAdd(pc, offset);
	pc = WrappIntSub(pc, 4);
};

void _cpu::OpBne(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	uint32_t s = instruction.s();
	_regIndex t = instruction.RegIndex();

	if (Reg(s) != Reg(t)) {
		Branch(i);
	}
};

void _cpu::OpAddi(_instruction instruction) {
	uint32_t i = (int32_t)instruction.SignExt();
	uint32_t t = instruction.RegIndex;
	uint32_t s = instruction.s();
	s = (int32_t)Reg(s);

	uint32_t answ = CheckedAdd(s, i);
	uint32_t v;
	switch (answ) {
		case 0:
			std::cout << "OpAddi overflow" << std::endl;
			break;
		default:
			v = (uint32_t)answ;
			break;
		}
	SetReg(t, v);
};

//OpLw load Word
void _cpu::OpLw(_instruction instruction) {
	if (StatReg & 0x10000 !=0) {
		std::cout << "Ignoring_load_while_cache_is_isolated" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	uint32_t s = instruction.s();
	
	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = Load32(addr);
	SetReg(t, v);
	d_regData = _registerData(t, v);
};

void _cpu::OpSltu(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) < Reg(t);
	SetReg(d, v);
};

void _cpu::OpAddu(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	
	uint32_t  v = WrappIntAdd(Reg(s), Reg(t));
	SetReg(d, v);
};

void _cpu::Store16(uint32_t addr, uint16_t val) {
	interconnect_c.Store16(addr, val);
};

void _cpu::OpSh(_instruction instruction) {
	if (StatReg & 0x10000 != 0) {
		std::cout << "Error in OpSh" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);

	Store16(addr, (uint16_t) v);
};

void _cpu::OpJal(_instruction instruction) {
	uint32_t ra = pc;
	SetReg(_regIndex(31), ra);
	OpJ(instruction);
};

void _cpu::OpAndi(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);

	Store8(addr, (uint8_t)v);
};

void _cpu::OpJr(_instruction instruction) {
	_regIndex s = instruction.s();
	pc = Reg(s);
};

