#include <isostream>
#include <cstdlib>
#include <vector>

#define BIOS_SIZE 512*1024

using namespace std;

class _CPU {
	protected: //becouse i dont give a fuck need i this or not
		uint32_t pc;
		uint32_t next_pc;
		uint32_t current_pc;
		uint32_t[32] regs;
		uint32_t hi;
		uint32_t lo;
		/*
		some non-standart definition
		tbd
		*/
		bool branch;
		bool delay_slot;
		bool debug_on_break;
	public:
	CPU() {
		pc=0xbfc00000;
		next_pc=
	}
	void set_debug_on_break(bool enable) {
		this.debug_on_break=enable;
	}
	void run_next_instruction() {
		auto pc =this.pc;
		auto instruction=this.load32(pc);
		this.pc=pc.wrappint_add(4);
		this.decode_and_execute(instruction);	
	}
	uint32_t wrappint_add(int incr) {

	}
	uint32_t.decode_and_execute(instruction) {

	}
}

class _bios {
	protected:	
		std::vector<uint32_t> data
	public:
	bios() {
			
	}//new is not constructor?
	uint32_t load32(uint32_t offset) {
		offset=(uint32_t) offset; //???
		uint32_t b0=data[offset+0];
		uint32_t b1=data[offset+1];
		uint32_t b2=data[offset+2];
		uint32_t b3=data[offset+3];

		return b0 | (b1 << 8) | (b2 << 16) | (b3<<24);
	}
}

class _interconnect {
	protected:	
		_bios bios;
	public:
	_intercinnect(_bios bios) {
	}		//?
	uint32_t load32(uint32_t addS)
}

		

