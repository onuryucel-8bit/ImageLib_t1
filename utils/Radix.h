#pragma once

#include<cstdint>
#include<string>
#include<sstream>

namespace rdx {

	/*
	* hex => dec
	* dec => bin
	* dec => hex
	*/

	char printDec_inhexType(int dec);

	//TODO 
	uint8_t bin_toDec(std::string bin);

	uint8_t hex_toDec(std::string hex);
	
	uint8_t hex_toDec(char hex);

	std::string dec_toBin(int dec);

	std::string dec_toHex(uint16_t dec);
}