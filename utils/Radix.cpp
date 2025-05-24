#include "Radix.h"

namespace rdx {

	//TODO !code here
	uint8_t bin_toDec(std::string bin) 
    {
		return 0;
	}

	uint8_t hex_toDec(std::string hex) 
    {
		
		//0xFE => FE

		unsigned int dec = std::stoul(hex, nullptr, 16);

		return (uint8_t)(dec);
	}

    char printDec_inhexType(int dec) {
        switch (dec) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return '?'; // or any other value indicating an error
        }
    }
   
    uint8_t hex_toDec(char hex) 
    {

        hex = std::toupper(hex);

        switch (hex) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
           
            return 200; 
        }
    }

	std::string dec_toBin(int dec) 
    {
		std::string bin = "";

		while (dec > 1) {

			bin += std::to_string(dec % 2);
			dec /= 2;
		}

		bin += std::to_string(dec);

		std::reverse(bin.begin(), bin.end());

		return bin;
	}

	std::string dec_toHex(uint16_t dec) 
    {
		std::stringstream ss;
		ss << std::hex << std::uppercase << dec;
		return ss.str();
	}
}