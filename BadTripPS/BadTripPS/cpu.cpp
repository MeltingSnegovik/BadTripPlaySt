#include <iostream>
#include <vector>

#include "cpu.h"


uint32_t _cpu:: WrappIntAdd(uint32_t pc, uint32_t incr) {
	if ((pc + incr) > 0xffffffff)
		return uint32_t()(pc + incr);
	else
		return 0x00000000;
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
	this->DecodeAndExecute(instruction_c);
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
		default:
			std::cout << "error on DecodeAndExecute" << std::endl;
			break;
	}
	// else return error
};

void _cpu::OpLui(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	uint32_t t = instruction.RegIndex();
	uint32_t v = i << 16;

	this->SetReg(t, v);
};

void _cpu::OpOri(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	uint32_t t = instruction.RegIndex();
	uint32_t s = instruction.s(); // tbd

	uint32_t v = this->Reg(s);
	this->SetReg(t, v);
};

void _cpu::OpSw(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	uint32_t t = instruction.RegIndex();
	uint32_t s = instruction.s(); //tbd
	
	uint32_t addr = WrappIntAdd(Reg(s),i);
	uint32_t v = Reg(t);
	return Store32(addr, v);
};

uint32_t _cpu::Reg(uint32_t index) {
	return regs_c[index];
};

void _cpu::SetReg(uint32_t index, uint32_t value) {
	regs_c[index] = value;
	regs_c[0] = 0;
};

void _cpu::Store32(uint32_t addr, uint32_t value) {
	interconnect_c.Store32(addr, value);
};

void _cpu::OpSll(_instruction instruction) {
	uint32_t i = instruction.Shift();
	uint32_t t = instruction.RegIndex();
	uint32_t d = instruction.RegInd15();

	uint32_t v = Reg(t) << i;
	SetReg(d, v);
};

void _cpu::OpAddiu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	uint32_t t = instruction.RegIndex();
	uint32_t s = instruction.s();

	uint32_t v = WrappIntAdd(Reg(s), i);
	SetReg(t, v);
};

void _cpu::OpJ(_instruction instruction) {
	uint32_t i = instruction.ImmJump();
	pc = ((pc & 0xf0000000) | (i << 2));
};

void _cpu::OpOr(_instruction instruction) {
	uint32_t d = instruction.RegInd15();
	uint32_t s = instruction.s();
	uint32_t t = instruction.RegIndex();

	uint32_t v = Reg(s) | Reg(t);
	SetReg(d, v);
};