#include "cpu.h"

#include <vector>


uint32_t _cpu:: WrappIntAdd(uint32_t pc, uint32_t incr) {
	if ((pc + incr) > 0xffffffff)
		return uint32_t()(pc + incr);
	else
		return 0x00000000;
};

void _cpu::SetDebugOnBreak(bool enable) {
	debugonbreak=enable;
};

void _cpu::RunNextInstruction() {
	instruction_c=this->load32(pc);
	pc= WrappIntAdd(pc,4);
	this.decode_and_execute(instruction_c);	
};


uint32_t _cpu::DecodeAndExecute(_instruction instruction)
{
	return uint32_t();
};
