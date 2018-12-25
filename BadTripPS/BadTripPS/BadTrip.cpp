#include <cstdio>
#include <string>
#include "bios.h"
#include "cpu.h"
#include "interconnect.h"
#include "map.h"
#include "wind.h"

/*
#include <SDL.h>
*/

int main(int argc, char** argv)
{
	//std::string pth = "D:\\cpp\\BadTripPlaySt\\BadTripPS\\BadTripPS\\SCPH1001.BIN";
	_bios Bios;
	_bios::BiosState bios_state = Bios.LoadBios();
	switch (bios_state)	{
	case _bios::BiosState::BIOS_STATE_INCORRECT_FILENAME:
		std::cout << "Can't find location of the bios "  << std::endl;
		return EXIT_FAILURE;
	case _bios::BiosState::BIOS_STATE_INVALID_BIOS_SIZE:
		std::cout << "Invalid BIOS size "  << std::endl;
		return EXIT_FAILURE;
	}
	
	pscx_wind::_wind wind;
	wind.InitW();
	_interconnect Inter= _interconnect(Bios,wind);
	_cpu Cpu = _cpu(Inter);
	while (true) {
			Cpu.RunNextInstruction();
	};

}