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
	pscx_memory::_regIndex RegIndex();

	// like a imm
	uint32_t ImmValue();

	// like a imm_se
	uint32_t SignExt();

	// like a d
	pscx_memory::_regIndex RegInd15();

	// like a shift
	uint32_t Shift();

	// like a immjump
	uint32_t ImmJump();

	//Return register index in bits [25;21]
	pscx_memory::_regIndex s();

	bool IsGteOP();
};