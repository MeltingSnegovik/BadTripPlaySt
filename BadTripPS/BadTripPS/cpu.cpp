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
	return interconnect_c.Load32(addr);
};

void _cpu::RunNextInstruction() {
	current_pc = pc;
	if (current_pc % 4 != 0) {
		Exception(_exception::e_LOADADDRESSERROR);
		return;
	}
	instruction_c= next_instruction;
	_instruction new_instruction(Load32(pc));
	next_instruction = new_instruction;
	pc= WrappIntAdd(pc,4);
	SetReg(d_regData.d_regIndex, d_regData.d_regValue);

	d_delay_slot = d_branch;
	d_branch = false;

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
	_regIndex s = instruction.s(); // tbd

	uint32_t v = this->Reg(s);
	this->SetReg(t, v);
};

//Store Word
void _cpu::OpSw(_instruction instruction) {
	if (StatReg & 0x1000 != 0) {
		std::cout << "error in OpSw" << std::endl;
//		return;
	}

	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s(); //tbd
	
	uint32_t addr = WrappIntAdd(Reg(s),i);
	uint32_t v = Reg(t);
	if (addr % 4 == 0) {
		Store32(addr, v);
	}
	else {
		Exception(_exception::e_STOREADDRESSERROR);
	};

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
	_regIndex s = instruction.s();

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
	SetReg(instruction.RegInd15(), Reg(instruction.s()) | Reg(instruction.RegIndex()));
};

void _cpu::OpCop0(_instruction instruction) {
	switch (instruction.CopOpCode()) {
	case 0b00000:
		this->OpMfc0(instruction);
		break;
	case 0b00100:
		this->OpMtc0(instruction);
		break;
	case 0b10000:
		this->OpRfe(instruction);
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
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	if (Reg(s) != Reg(t)) {
		Branch(i);
	}
};

void _cpu::OpAddi(_instruction instruction) {
	int32_t i = (int32_t)instruction.SignExt();
	uint32_t t = instruction.RegIndex;
	_regIndex s = instruction.s();
	s = (int32_t)Reg(s);

	uint32_t answ = CheckedAdd(s.m_index,(uint32_t)i); //wtf?
	uint32_t v;
	switch (answ) {
		case 0:
			Exception(_exception::e_OVERFLOW);
			break;
		default:
			v = (uint32_t)answ;
			SetReg(t, v);
			break;
		}
	
};

//OpLw load Word
void _cpu::OpLw(_instruction instruction) {
	if (StatReg & 0x10000 !=0) {
		std::cout << "Ignoring_load_while_cache_is_isolated" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();
	
	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v;
	if (addr % 4 == 0) {
		Load32(addr);
		SetReg(t, v);
		d_regData = _registerData(t, v);
	} 
	else {
		Exception(_exception::e_LOADADDRESSERROR);
	};
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

	if (addr % 2 == 0) {
		Store16(addr, (uint16_t)v);
	}
	else {
		Exception(_exception::e_STOREADDRESSERROR);
	};

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

uint8_t _cpu::Load8(uint32_t addr) {
	interconnect_c.Load32(addr);
};

void _cpu::OpLb(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	int32_t v = (int8_t)Load8(addr);
	d_regData= _registerData(t, (uint32_t)v);
};

void _cpu::OpBeq(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	if (Reg(s) == Reg(t)) {
		Branch(i);
	};
}

void _cpu::OpMfc0(_instruction instruction) {
	_regIndex cpu_r = instruction.RegIndex();
	_regIndex cop_r = instruction.RegInd15();

	uint32_t v;
	switch (cop_r.m_index) {
	case 12:
		v = StatReg;
		break;
	case 13:
		v = d_cause;
//		std::cout << "Unhandled_read_from_CAUSE_register" << std::endl;
		break;
	case 14:
		v = d_epc;
		break;
	default:
		std::cout << "Unhandled_read_from_cop0r" << cop_r.m_index << std::endl;
		break;
	};
	d_regData= _registerData(cpu_r, v);
};

void _cpu::OpAnd(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) & Reg(t);
	SetReg(d,v);
};

void _cpu::OpAdd(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	int32_t s_int = (int32_t)Reg(s);
	int32_t t_int = (int32_t)Reg(t);
	uint32_t v = (uint32_t)CheckedAdd(s_int, t_int);
	if (v != (-1))
		SetReg(d, v);
	else
		Exception(_exception::e_OVERFLOW);

};

void _cpu::OpBgtz(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	uint32_t v = Reg(s);
	
	if (v > 0) {
		Branch(i);	
	}
};

void _cpu::OpBlez(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	
	int32_t v =(int32_t)Reg(s);
	if (v<=0){
		Branch(i);		
	}
};

void _cpu::OpLbu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();
	
	uint32_t addr = WrappIntAdd(Reg(s),i);
	uint8_t v = Load8(addr);
	d_regData = _registerData(t,(uint32_t)v);
};

void _cpu::OpJalr(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	
	uint32_t ra=pc;
	SetReg(d,ra);
	pc=Reg(s);
};

void _cpu::OpBxx(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	
	uint32_t d_instr = instruction_c.data;
	
	uint32_t is_bgez = (d_instr >> 16) & 1;
	uint32_t is_link = (d_instr >> 17) & 0xf; //==8 tbd
	
	int32_t v = (int32_t)Reg(s);
	uint32_t test;
	if (v<0)
		test = 1;
	else
		test = 0;
	
	uint32_t ra;
	_regIndex RegInd(31);
	test = test^is_bgez;
	if (is_link){
		ra=pc;
		SetReg(RegInd,ra);
	}
	
	if (test!=0){
		Branch(i);
	}
};

void _cpu::OpSlti(_instruction instruction) {
	int32_t i =(int32_t)instruction.SignExt();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	
	uint32_t v;
	if ((int32_t)Reg(s) < i)
		v=1;
	else 
		v=0;
	SetReg(t,(uint32_t)v);
};

void _cpu::OpSubu(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	_regIndex d = instruction.RegInd15();
	
	uint32_t v = WrappIntAdd(Reg(s),Reg(t));
	SetReg(d,v);
};

void _cpu::OpSra(_instruction instruction) {
	uint32_t i = instruction.Shift();
	_regIndex t = instruction.RegIndex();
	_regIndex d = instruction.RegInd15();
	
	int32_t v = (int32_t)Reg(t)>>i;
	SetReg(d,(uint32_t)v);
};

void _cpu::OpDiv(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	
	int32_t n = (int32_t)Reg(s);
	int32_t d = (int32_t)Reg(t);
	
	if (d==0) {
		d_hi=(uint32_t)n;
		
		if (n>=0)
			d_lo=0xffffffff;
		else
			d_lo=1;
	}
	else if ((uint32_t)n==0x80000000 && d==-1) {
		d_hi=0;
		d_lo=0x80000000;
	}
	else {
		d_hi=(uint32_t) n%d;
		d_lo=(uint32_t) n/d;
	}
};

void _cpu::OpMflo(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	SetReg(d, d_lo);
};

void _cpu::OpSrl(_instruction instruction) {
	uint32_t i = instruction.Shift();
	_regIndex t = instruction.RegIndex();
	_regIndex d = instruction.RegInd15();
	
	uint32_t v = Reg(t)>> i;
	SetReg(d,v);
};

void _cpu::OpSltiu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	
	uint32_t v= (uint32_t)(Reg(s)<i);	
	SetReg(t,v);
};

void _cpu::OpDivu(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	
	uint32_t n = Reg(s);
	uint32_t d = Reg(t);
	
	if (d==0){
		d_hi = n;
		d_lo = 0xffffffff;
	}
	else {
		d_hi = n%d;
		d_lo = n/d;
	}
};

void _cpu::OpMvhi(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	SetReg(d, d_hi);
};

void _cpu::OpSlt(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	int32_t t_s = (int32_t)Reg(s);
	int32_t t_t = (int32_t)Reg(t);

	uint32_t v = (uint32_t)(t_s < t_t);
	SetReg(d, v);
};

void _cpu::Exception(_exception cause) {
	uint32_t handl;
	if (StatReg & (1 << 22) != 0)
		handl = 0xbfc00180;
	else
		handl = 0x80000080;
	auto mode = StatReg & 0x3f;
	StatReg &= 0x3f;
	StatReg |= (mode << 2) & 0x3f;

	d_cause = d_cause << 2;
	d_epc = current_pc;
	pc = handl;
	next_pc = WrappIntAdd(pc, 4);

	//
	cause = (_exception)((uint32_t)cause << 2);
	d_epc = current_pc;
	if (d_delay_slot) {
		d_epc = WrappIntAdd(d_epc, 4);
		d_cause |= 1 << 31;
	}
};

void _cpu::OpSysCall(_instruction instruction) {
	Exception(e_SYSCALL);
};

void _cpu::OpMtlo(_instruction instruction) {
	_regIndex s = instruction.s();
	d_lo = Reg(s);
};

void _cpu::OpMthi(_instruction instruction) {
	_regIndex s = instruction.s();
	d_hi = Reg(s);
};

void _cpu::OpRfe(_instruction instruction) {
	if (instruction.data & 0x3f != 0b010000)
		return;
	auto mode = StatReg & 0x3f;
	StatReg &= !0x3f;
	StatReg |= mode >> 2;
};

uint16_t _cpu::Load16(uint32_t addr) {
	interconnect_c.Load16(addr);
	return 0;
};

void _cpu::OpLhu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v;
	if (addr % 2 == 0) {
		v = (uint32_t)Load16(addr);
		d_regData = _registerData(t, v);
	}
	else {
		Exception(_exception::e_LOADADDRESSERROR);
	};
};

void _cpu::OpSlly(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	auto v = Reg(t) << (Reg(s) & 0x1f);
	SetReg(d, v);
};

void _cpu::OpLh(_instruction instruction) { 
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = (int16_t)Load16(addr);
	d_regData = _registerData(t, v);
};

void _cpu::OpNor(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	
	uint32_t v = !(Reg(s) | Reg(t));
	SetReg(d, v);
};

void _cpu::OpSrav(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t v = (((int32_t)Reg(t)) >> (Reg(s) & 0x1f));
	SetReg(d, v);
};

void _cpu::OpSrlv(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t v = (Reg(t) >> (Reg(s) & 0x1f));
	SetReg(d, v);
};


void _cpu::OpMultu(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	uint64_t a = Reg(s);
	uint64_t b = Reg(t);
	auto v=a*b;
	d_hi = (uint32_t)v >> 32;
	d_lo = (uint32_t)v;
};

void _cpu::OpXor(_instruction instruction) {
	_regIndex d = instruction.RegInd15();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) ^ Reg(t);
	SetReg(d, v);
};

void _cpu::OpBreak(_instruction instruction) {
	Exception(_exception::e_BREAK);
};

void _cpu::OpMult(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	int64_t a = (int64_t)Reg(s);
	int64_t b = (int64_t)Reg(t);

	uint64_t v = a*b;

	d_hi = (uint32_t)(v >> 32);
	d_lo = (uint32_t)(v);
};

void _cpu::OpSub(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	_regIndex d = instruction.RegInd15();

	int32_t a = (int32_t)Reg(s);
	int32_t b = (int32_t)Reg(t);

	int32_t v = CheckedSub(a, b); //tbd

	if (v != -1) {
		SetReg(d, v);
	}
	else {
		Exception(_exception::e_OVERFLOW);
	}
};


void _cpu::OpXori(_instruction instruction) {
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();
	uint32_t i = instruction.ImmValue();

	uint32_t v = Reg(s) ^ i;

	SetReg(t, v);
};

void _cpu::OpCop1(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpCop3(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpLwl(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex s = instruction.s();
	_regIndex t = instruction.RegIndex();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t cur_v = out_regs[t.m_index];

	uint32_t al_addr = addr & !3;
	uint32_t al_word = this->Load32(al_addr);

	uint32_t v;
	switch (addr & 3) {
	case 0:
		v = (cur_v & 0x00ffffff) | (al_word << 24);
		break;
	case 1:
		v = (cur_v & 0x0000ffff) | (al_word << 16);
		break;
	case 2:
		v = (cur_v & 0x000000ff) | (al_word << 8);
		break;
	case 3:
		v = (cur_v & 0x00ffffff) | (al_word << 0);
		break;
	};

	d_regData = _registerData(t, v);

};

void _cpu::OpLwr(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t cur_v = out_regs[t.m_index];

	uint32_t al_addr = addr & !3;
	uint32_t al_word = Load32(al_addr);

	uint32_t v;
	switch (addr & 3) {
	case 0:
		v = (cur_v & 0x00000000) | (al_word >> 0);
		break;
	case 1:
		v = (cur_v & 0xff000000) | (al_word >> 8);
		break;
	case 2:
		v = (cur_v & 0xffff0000) | (al_word >> 16);
		break;
	case 3:
		v = (cur_v & 0xffffff00) | (al_word >> 24);
		break;
	}
	d_regData = _registerData(t, v);
};

void _cpu::OpSwl(_instruction instruction) {  
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);
	uint32_t al_addr = addr& !3;

	uint32_t cur_mem = Load32(al_addr);
	uint32_t mem;	//kek
	switch (addr & 3) {
	case 0:
		mem = (cur_mem & 0xffffff00) | (v >> 24);
		break;
	case 1:
		mem = (cur_mem & 0xffff0000) | (v >> 16);
		break;
	case 2:
		mem = (cur_mem & 0xff000000) | (v >> 8);
		break;
	case 3:
		mem = (cur_mem & 0x00000000) | (v >> 0);
		break;
	};
	Store32(al_addr, mem);
};



void _cpu::OpSwr(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	_regIndex t = instruction.RegIndex();
	_regIndex s = instruction.s();

	uint32_t addr = WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);
	uint32_t al_addr = addr& !3;

	uint32_t cur_mem = Load32(al_addr);
	uint32_t mem;	//kek
	switch (addr & 3) {
	case 0:
		mem = (cur_mem & 0x00000000) | (v << 0);
		break;
	case 1:
		mem = (cur_mem & 0x000000ff) | (v << 8);
		break;
	case 2:
		mem = (cur_mem & 0x0000ffff) | (v << 16);
		break;
	case 3:
		mem = (cur_mem & 0x00ffffff) | (v << 24);
		break;
	};
	Store32(al_addr, mem);
};