#pragma once

#include <stdint.h>
#include "map.h"


struct _instruction {
public:
	_instruction(uint32_t t_data) :
		data(t_data)
	{};
	uint32_t data;
	// like a funtion
	uint32_t Function();

	// like a subfunction
	uint32_t SubFunction();

	//like a coprocessor
	uint32_t CopOpCode();

	// like a t
	uint32_t RegIndex();	

	// like a imm
	uint32_t ImmValue();

	// like a imm_se
	uint32_t SignExt();

	// like a d
	uint32_t RegInd15();

	// like a shift
	uint32_t Shift();

	// like a immjump
	uint32_t ImmJump();

};