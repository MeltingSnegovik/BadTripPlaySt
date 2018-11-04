#pragma once

#include "instruction.h"

uint32_t _instruction::Function() { 
	return data>>26;
}
uint32_t _instruction::Register() {
	return (data>>16) & 0x1f
}
uint32_t _instruction::ImmediateValue() {
	return data & 0xffff
}
