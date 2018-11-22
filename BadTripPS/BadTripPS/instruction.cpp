#pragma once

#include "instruction.h"

// function
uint32_t _instruction::Function() { 
	return data>>26;
};

// like a subfunction
uint32_t _instruction::SubFunction() {
	return data & 0x3f;
};

//like a coprocessor
uint32_t _instruction::CopOpCode() {
	return (data >> 21) & 0x1f;
};

// like a t
uint32_t _instruction::RegIndex() {
	return (data >> 16) & 0x1f;
};

// like a imm
uint32_t _instruction::ImmValue() {
	return data & 0xffff;
};

// like a imm_se
uint32_t _instruction::SignExt() {
	uint16_t v = (data & 0xffff);
	return (uint32_t)v;
};

// like a d
uint32_t _instruction::RegInd15() {
	return (data >> 11) & 0x1f;
};

// like a shift
uint32_t _instruction::Shift() {
	return (data >> 6) & 0x1f;
};

// like a immjump
uint32_t _instruction::ImmJump() {
	return (data & 0x3ffffff);
};