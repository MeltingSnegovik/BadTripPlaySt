#include <cstdio>
#include <string>
#include "bios.h"
#include "cpu.h"
#include "interconnect.h"
#include "map.h"


int main(int argc, char** argv)
{
	std::string pth = "D:\cpp\BadTripPlaySt\BadTripPS\BadTripPS\SCPH1001.BIN";
	_bios Bios;
	_bios::BiosState bios_state = Bios.LoadBios(pth);
	
	_interconnect Inter(Bios);
	_cpu Cpu(Inter);
	while (true) {
		Cpu.RunNextInstruction();
	};

}