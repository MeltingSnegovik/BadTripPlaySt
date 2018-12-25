#include "cpu.h"

void _cpu::SetDebugOnBreak(bool enable) {
	debugonbreak=enable;
};

_instruction _cpu::Load32(uint32_t addr) {
	return interconnect_c.Load32(addr);
};
_instruction _cpu::Load16(uint32_t addr) {
	return interconnect_c.Load16(addr);
};
_instruction _cpu::Load8(uint32_t addr) {
	return interconnect_c.Load8(addr);
};
void _cpu::Store32(uint32_t addr, uint32_t value) {
	interconnect_c.Store32(addr, value);
};
void _cpu::Store16(uint32_t addr, uint16_t val) {
	interconnect_c.Store16(addr, val);
};
void _cpu::Store8(uint32_t addr, uint8_t val) {
	interconnect_c.Store8(addr, val);
};

void _cpu::SetPC(uint32_t f_pc) {
	pc = f_pc;
	next_pc = f_pc;
};

void _cpu::RunNextInstruction() {
	current_pc = pc;
	if (current_pc % 4 != 0) {
		Exception(_exception::e_LOADADDRESSERROR);
		return;
	}
	instruction_c= next_instruction;
	_instruction new_instruction = _cpu::Load32(pc);
	next_instruction = new_instruction;
	pc = next_pc;
	next_pc =pscx_rustf::WrappIntAdd(next_pc, 4);
	SetReg(d_regData.d_regIndex, d_regData.d_regValue);

	d_delay_slot = d_branch;
	d_branch = false;
	d_regData = _registerData(pscx_memory::_regIndex(0x0), 0x0);
	this->DecodeAndExecute(instruction_c);
	//in rust regs_c = out_regs;
	//std::copy(std::begin(out_regs), std::end(out_regs), std::begin(regs_c));

};

void _cpu::DecodeAndExecute(_instruction instruction) {
	uint32_t temp_reg; 
	golden_res >> temp_reg;
	file_out << instruction.data <<" " << temp_reg << "\n";
	std::cout << instruction.data << " " << temp_reg << std::endl;
	uint32_t retInstrFunct = instruction.Function();
	switch (retInstrFunct) {
		case 0b000000:
			switch (instruction.SubFunction()) {
				case 0b000000:
					OpSll(instruction);
					break;
				case 0b000010:
					OpSrl(instruction);
					break;
				case 0b000011:
					OpSra(instruction);
					break;
				case 0b000100:
					OpSllv(instruction);
					break;
				case 0b000110:
					OpSrlv(instruction);
					break;
				case 0b000111:
					OpSrav(instruction);
					break;
				case 0b001000:
					OpJr(instruction);
					break;
				case 0b001001:
					OpJalr(instruction);
					break;
				case 0b001100:
					OpSysCall(instruction);
					break;
				case 0b001101:
					OpBreak(instruction);
					break;
				case 0b010000:
					OpMfhi(instruction);
					break;
				case 0b010001:
					OpMthi(instruction);
					break;
				case 0b010010:
					OpMflo(instruction);
					break;
				case 0b010011:
					OpMtlo(instruction);
					break;
				case 0b011000:
					OpMult(instruction);
					break;
				case 0b011001:
					OpMultu(instruction);
					break;
				case 0b011010:
					OpDiv(instruction);
					break;
				case 0b011011:
					OpDivu(instruction);
					break;
				case 0b100000:
					OpAdd(instruction);
					break;
				case 0b100001:
					OpAddu(instruction);
					break;
				case 0b100010:
					OpSub(instruction);
					break;
				case 0b100011:
					OpSubu(instruction);
					break;
				case 0b100100:
					OpAnd(instruction);
					break;
				case 0b100101:
					OpOr(instruction);
					break;
				case 0b100110:
					OpXor(instruction);
					break;
				case 0b100111:
					OpNor(instruction);
					break;
				case 0b101010:
					OpSlt(instruction);
					break;
				case 0b101011:
					OpSltu(instruction);
					break;
				default:
					OpIllegal(instruction); //tbd
					break;
			};
			break;
		case 0b000001:
			OpBxx(instruction);
			break;
		case 0b000010:
			OpJ(instruction);
			break;
		case 0b000011:
			OpJal(instruction);
			break;
		case 0b000100:
			OpBeq(instruction);
			break;
		case 0b000101:
			OpBne(instruction);
			break;
		case 0b000110:
			OpBlez(instruction);
			break;
		case 0b000111:
			OpBgtz(instruction);
			break;
		case 0b001000:
			OpAddi(instruction);
			break;
		case 0b001001:
			OpAddiu(instruction);
			break;
		case 0b001010:
			OpSlti(instruction);
			break;
		case 0b001011:
			OpSltiu(instruction);
			break;
		case 0b001100:
			OpAndi(instruction);
			break;
		case 0b001101:
			OpOri(instruction);
			break;
		case 0b001110:
			OpXori(instruction);
			break;
		case 0b001111:
			OpLui(instruction);
			break;
		case 0b010000:
			OpCop0(instruction);
			break;
		case 0b010001:
			OpCop1(instruction);
			break;
		case 0b010010:
			break;
		case 0b010011:
			OpCop3(instruction);
			break;
		case 0b100000:
			OpLb(instruction);
			break;
		case 0b100001:
			OpLh(instruction);
			break;
		case 0b100010:
			OpLwl(instruction);
			break;
		case 0b100011:
			OpLw(instruction);
			break;
		case 0b100100:
			OpLbu(instruction);
			break;
		case 0b100101:
			OpLhu(instruction);
			break;
		case 0b100110:
			OpLwr(instruction);
			break;
		case 0b101000:
			OpSb(instruction);
			break;
		case 0b101001:
			OpSh(instruction);
			break;
		case 0b101010:
			OpSwl(instruction);
			break;
		case 0b101011:
			OpSw(instruction);
			break;
		case 0b101110:
			OpSwr(instruction);
			break;
		case 0b110000:
			OpLwc0(instruction);
			break;
		case 0b110001:
			OpLwc1(instruction);
			break;
		case 0b110010:
			OpLwc2(instruction);
			break;
		case 0b110011:
			OpLwc3(instruction);
			break;
		case 0b111000:
			OpSwc0(instruction);
			break;
		case 0b111001:
			OpSwc1(instruction);
			break;
		case 0b111010:
			OpSwc2(instruction);
			break;
		case 0b111011:
			OpSwc3(instruction);
			break;
		default:
			OpIllegal(instruction);
			break;
	};
	// else return error
};

void _cpu::OpLui(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	pscx_memory::_regIndex t = instruction.RegIndex();
	uint32_t v = i << 16;
	DelayedLoad();
	SetReg(t, v);
};

void _cpu::OpOri(_instruction instruction) {
	uint32_t i = instruction.ImmValue();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s(); 

	uint32_t v = Reg(s) | i;
	DelayedLoad();
	SetReg(t, v);
};

//Store Word
void _cpu::OpSw(_instruction instruction) {
	if (StatReg & 0x1000 != 0) {
		std::cout << "error in OpSw" << std::endl;
//		return;
	}

	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s(); //tbd
	
	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s),i);
	uint32_t v = Reg(t);
	if (addr % 4 == 0) {
		Store32(addr, v);
	}
	else {
		Exception(_exception::e_STOREADDRESSERROR);
	};

};

uint32_t _cpu::Reg(pscx_memory::_regIndex index) {
	assert(index.m_index < _countof(regs_c), "Reg Overflow");
	return regs_c[index.m_index];
};

void _cpu::SetReg(pscx_memory::_regIndex index, uint32_t value) {
	assert(index.m_index < _countof(regs_c), "Set Reg Overflow");
	regs_c[index.m_index] = value;
	regs_c[0] = 0;
};

void _cpu::OpSll(_instruction instruction) {
	uint32_t i = instruction.Shift();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex d = instruction.RegInd15();

	uint32_t v = Reg(t) << i;
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpAddiu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t v = pscx_rustf::WrappIntAdd(Reg(s), i);
	DelayedLoad();
	SetReg(t, v);
};

void _cpu::OpJ(_instruction instruction) {
	uint32_t i = instruction.ImmJump();
	next_pc = ((pc & 0xf0000000) | (i << 2));
	d_branch = true;
	DelayedLoad();
};

void _cpu::OpOr(_instruction instruction) {
	
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	
	uint32_t v = Reg(s) | Reg(t);
	DelayedLoad();

	SetReg(d, v);
	//SetReg(instruction.RegInd15(), Reg(instruction.s()) | Reg(instruction.RegIndex()));
};

void _cpu::OpCop0(_instruction instruction) {
	switch (instruction.CopOpCode()) {
	case 0b00000:
		OpMfc0(instruction);
		break;
	case 0b00100:
		OpMtc0(instruction);
		break;
	case 0b10000:
		OpRfe(instruction);
		break;
	default:
		std::cout << "error in OpCop0" << std::endl;
		break;
	};
};

void _cpu::OpMtc0(_instruction instruction) {
	pscx_memory::_regIndex cpu_r = instruction.RegIndex();
	pscx_memory::_regIndex cop_r = instruction.RegInd15();

	uint32_t v = this->Reg(cpu_r);
	DelayedLoad();
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
	auto nw = offset << 2;
	next_pc = pscx_rustf::WrappIntAdd(pc, nw);
	d_branch = true;
	//pc = pscx_rustf::WrappIntSub(pc, 4);
};

void _cpu::OpBne(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	if (Reg(s) != Reg(t)) {
		Branch(i);
	};
	DelayedLoad();
};

void _cpu::OpAddi(_instruction instruction) {
	int32_t i = (int32_t)instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	int32_t t_s = (int32_t)Reg(s);
	DelayedLoad();
	uint32_t v;
	if (pscx_rustf::CheckedAdd(t_s, i))	{
		v = (uint32_t)(t_s + i);
		SetReg(t, v);
	}	
	else {
		Exception(_exception::e_OVERFLOW);
	};
};

//OpLw load Word
void _cpu::OpLw(_instruction instruction) {
	if (StatReg & 0x10000 !=0) {
		std::cout << "Ignoring_load_while_cache_is_isolated" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();
	
	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	_instruction temp_i = Load32(addr);
	uint32_t v;
	if (addr % 4 == 0) {
		v=temp_i.data;
		//SetReg(t, v);  // tbd
		d_regData = _registerData(t, v);
	} 
	else {
		DelayedLoad();
		Exception(_exception::e_LOADADDRESSERROR);
	};
};

void _cpu::OpSltu(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) < Reg(t);
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpAddu(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	
	uint32_t  v = pscx_rustf::WrappIntAdd(Reg(s), Reg(t));
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpSh(_instruction instruction) {
	if (StatReg & 0x10000 != 0) {
		std::cout << "Error in OpSh" << std::endl;
		return;
	}

	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);
	DelayedLoad();

	if (addr % 2 == 0) {
		Store16(addr, (uint16_t)v);
	}
	else {
		Exception(_exception::e_STOREADDRESSERROR);
	};
};

void _cpu::OpJal(_instruction instruction) {
	uint32_t ra = pc;
	OpJ(instruction);
	SetReg(pscx_memory::_regIndex(31), ra);
	d_branch = true;
};

//op sb
void _cpu::OpSb(_instruction instruction) {
	if (StatReg & 0x10000 != 0) {
		std::cout << "error in OpSb" << std::endl;
		return;
	};

	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);

//	if (addr % 2 == 0) {
	DelayedLoad();
		Store8(addr, (uint8_t)v);
/*	
}
	else {
		Exception(_exception::e_STOREADDRESSERROR);
	};
*/

};

void _cpu::OpAndi(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t v = Reg(s) &i;
	DelayedLoad();
	SetReg(t, v);
};

void _cpu::OpJr(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	next_pc = Reg(s);
	d_branch = true;
};

void _cpu::OpLb(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	_instruction temp_i = Load8(addr);
	uint8_t v = temp_i.data;
	d_regData= _registerData(t, (uint32_t)v);
};

void _cpu::OpBeq(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	if (Reg(s) == Reg(t)) {
		Branch(i);
	};
	DelayedLoad();
};

void _cpu::OpMfc0(_instruction instruction) {
	pscx_memory::_regIndex cpu_r = instruction.RegIndex();
	pscx_memory::_regIndex cop_r = instruction.RegInd15();
	
	uint32_t v=0; //tbd
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
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) & Reg(t);
	SetReg(d,v);
};

void _cpu::OpAdd(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	int32_t s_int = (int32_t)Reg(s);
	int32_t t_int = (int32_t)Reg(t);
	DelayedLoad();
	if (pscx_rustf::CheckedAdd(s_int, t_int))
		SetReg(d, (uint32_t)(s_int + t_int));
	else {
		Exception(_exception::e_OVERFLOW);
		SetReg(d, 0);  //tbd
	}
};

void _cpu::OpBgtz(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	int32_t v = (int32_t)Reg(s);
	
	if (v > 0) {
		Branch(i);	
	};
	DelayedLoad();
};

void _cpu::OpBlez(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	
	int32_t v =(int32_t)Reg(s);
	if (v<=0){
		Branch(i);		
	};
	DelayedLoad();
};

void _cpu::OpLbu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();
	
	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s),i);
	_instruction temp_i = Load8(addr);
	uint8_t v = temp_i.data;
	d_regData = _registerData(t,(uint32_t)v);
};

void _cpu::OpJalr(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	
	uint32_t ra=next_pc;
	next_pc = Reg(s);
	DelayedLoad();
	SetReg(d,ra);

	d_branch = true;
	//pc=Reg(s);
};

//branch if less than zero
//branch if less zero and link
//branch if great than or equal to zero
//branch if great than or equal to zero and link

void _cpu::OpBxx(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	
	uint32_t d_instr = instruction_c.data;
	
	uint32_t is_bgez = (d_instr >> 16) & 1;
	bool is_link = ((d_instr >> 17) & 0xf == 8);
	
	int32_t v = (int32_t)Reg(s);
	uint32_t test = (uint32_t)(v<0);
	
	uint32_t ra;
	pscx_memory::_regIndex RegInd(31);
	test = test^is_bgez;
	DelayedLoad();
	if (is_link){
		ra=next_pc;
		SetReg(RegInd,ra);
	}
	
	if (test!=0){
		Branch(i);
	}
};

void _cpu::OpSlti(_instruction instruction) {
	int32_t i =(int32_t)instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	DelayedLoad();
	SetReg(t, (uint32_t)((int32_t)Reg(s) < i)); //tbd
};

void _cpu::OpSubu(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex d = instruction.RegInd15();
	
	uint32_t v = pscx_rustf::WrappIntSub(Reg(s),Reg(t));
	DelayedLoad();
	SetReg(d,v);
};

void _cpu::OpSra(_instruction instruction) {
	uint32_t i = instruction.Shift();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex d = instruction.RegInd15();
	
	int32_t v = ((int32_t)Reg(t))>>i;
	DelayedLoad();
	SetReg(d,(uint32_t)v);
};

void _cpu::OpDiv(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	
	int32_t n = (int32_t)Reg(s);
	int32_t d = (int32_t)Reg(t);
	
	DelayedLoad();

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
	pscx_memory::_regIndex d = instruction.RegInd15();
	DelayedLoad();
	SetReg(d, d_lo);
};

//tbd
void _cpu::OpMfhi(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	DelayedLoad();
	SetReg(d, d_hi);
};

//shift right logical
void _cpu::OpSrl(_instruction instruction) {
	uint32_t i = instruction.Shift();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex d = instruction.RegInd15();

	uint32_t v = Reg(t) >> i;
	DelayedLoad();
	SetReg(d,v);
};

//set less than immediate unsigned
void _cpu::OpSltiu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	
	uint32_t v= (uint32_t)(Reg(s)<i);	
	DelayedLoad();
	SetReg(t,v);
};

void _cpu::OpDivu(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	
	uint32_t n = Reg(s);
	uint32_t d = Reg(t);
	
	DelayedLoad();
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
	pscx_memory::_regIndex d = instruction.RegInd15();
	SetReg(d, d_hi);
};

void _cpu::OpSlt(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	int32_t t_s = (int32_t)Reg(s);
	int32_t t_t = (int32_t)Reg(t);

	DelayedLoad();

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

	d_cause = (uint32_t)cause << 2; //tbd
	d_epc = current_pc;
	pc = handl;
	next_pc = pscx_rustf::WrappIntAdd(pc, 4);

	//
	//d_cause = (_exception)((uint32_t)cause << 2);  //tbd
	if (d_delay_slot) {
		d_epc = pscx_rustf::WrappIntAdd(d_epc, 4);
		d_cause |= 1 << 31;
	};
};

void _cpu::OpSysCall(_instruction instruction) {
	DelayedLoad();
	Exception(e_SYSCALL);
};

void _cpu::OpMtlo(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	d_lo = Reg(s);
};

void _cpu::OpMthi(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	d_hi = Reg(s);
	DelayedLoad();
};


//tbd
void _cpu::OpRfe(_instruction instruction) {
	DelayedLoad();
	if (instruction.data & 0x3f != 0b010000)
		std::cout << "invalid cp0 instr " << instruction.data << std::endl;
		//	return;	//tbd
	auto mode = StatReg & 0x3f;
	StatReg &= ~0x3f; //tbd
	StatReg |= mode >> 2;
};



void _cpu::OpLhu(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t v;
	if (addr % 2 == 0) {
		_instruction temp_i = Load16(addr);
		v = (uint32_t)temp_i.data;
		d_regData = _registerData(t, v);
	}
	else {
		DelayedLoad();
		Exception(_exception::e_LOADADDRESSERROR);
	};
};

void _cpu::OpSllv(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(t) << (Reg(s) & 0x1f);
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpLh(_instruction instruction) { 
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	_instruction temp_i = Load16(addr);
	uint32_t v = temp_i.data;
	d_regData = _registerData(t, v);
};

void _cpu::OpNor(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	
	uint32_t v = ~(Reg(s) | Reg(t)); //bn
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpSrav(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v =(uint32_t)(((int32_t)Reg(t)) >> (Reg(s) & 0x1f));
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpSrlv(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v = (Reg(t) >> (Reg(s) & 0x1f));
	DelayedLoad();
	SetReg(d, v);
};


void _cpu::OpMultu(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	uint64_t a = Reg(s);
	uint64_t b = Reg(t);
	DelayedLoad();
	auto v=a*b;
	d_hi = (uint32_t)v >> (uint32_t)32;
	d_lo = (uint32_t)v;
};

void _cpu::OpXor(_instruction instruction) {
	pscx_memory::_regIndex d = instruction.RegInd15();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t v = Reg(s) ^ Reg(t);
	DelayedLoad();
	SetReg(d, v);
};

void _cpu::OpBreak(_instruction instruction) {
	DelayedLoad();
	Exception(_exception::e_BREAK);
};

void _cpu::OpMult(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	int64_t a = (int64_t)Reg(s);
	int64_t b = (int64_t)Reg(t);

	DelayedLoad();

	uint64_t v = a*b;

	d_hi = (uint32_t)(v >> 32);
	d_lo = (uint32_t)(v);
};

void _cpu::OpSub(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex d = instruction.RegInd15();

	int32_t a = (int32_t)Reg(s);
	int32_t b = (int32_t)Reg(t);
	DelayedLoad();

	if (pscx_rustf::CheckedSub(a, b))
		SetReg(d, (uint32_t)(a-b));
	else
		Exception(_exception::e_OVERFLOW);
};


void _cpu::OpXori(_instruction instruction) {
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();
	uint32_t i = instruction.ImmValue();

	uint32_t v = Reg(s) ^ i;
	DelayedLoad();
	SetReg(t, v);
};

void _cpu::OpCop1(_instruction instruction) {
	DelayedLoad();
	Exception(_exception::e_COPROCESSORERROR);
};


void _cpu::OpCop2(_instruction instruction) {
	std::cout << "unhandled gte instruction" << std::endl;
};

void _cpu::OpCop3(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpLwl(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex s = instruction.s();
	pscx_memory::_regIndex t = instruction.RegIndex();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t cur_v = Reg(t);

	uint32_t al_addr = addr & ~3;
	_instruction temp_i = Load32(al_addr);
	uint32_t al_word = temp_i.data;

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
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t cur_v = Reg(t);

	uint32_t al_addr = addr & ~3;
	_instruction temp_i = Load32(al_addr);
	uint32_t al_word = temp_i.data;

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
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);
	uint32_t al_addr = addr & ~3;
	_instruction temp_i = Load32(al_addr);
	uint32_t cur_mem = temp_i.data;
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
	DelayedLoad();
	Store32(al_addr, mem);
};



void _cpu::OpSwr(_instruction instruction) {
	uint32_t i = instruction.SignExt();
	pscx_memory::_regIndex t = instruction.RegIndex();
	pscx_memory::_regIndex s = instruction.s();

	uint32_t addr = pscx_rustf::WrappIntAdd(Reg(s), i);
	uint32_t v = Reg(t);
	uint32_t al_addr = addr & ~3;
	_instruction temp_i = Load32(al_addr);
	uint32_t cur_mem = temp_i.data;
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

void _cpu::OpLwc0(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpLwc1(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpLwc2(_instruction instruction) {
	std::cout << "unhandled GTE LWC" << instruction.data << std::endl;
};

void _cpu::OpLwc3(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpSwc0(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpSwc1(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpSwc2(_instruction instruction) {
	std::cout << "unhandled GTE SWC" << instruction.data << std::endl;
};

void _cpu::OpSwc3(_instruction instruction) {
	Exception(_exception::e_COPROCESSORERROR);
};

void _cpu::OpIllegal(_instruction instruction) {
	DelayedLoad();
	std::cout << "Illegal instruction" << instruction.data << std::endl;
	Exception(_exception::e_ILLEGALINSTRUCTION);
};

void _cpu::DelayedLoad() {
	SetReg(d_regData.d_regIndex, d_regData.d_regValue);
	d_regData = _registerData(pscx_memory::_regIndex(0x0), 0x0);
};