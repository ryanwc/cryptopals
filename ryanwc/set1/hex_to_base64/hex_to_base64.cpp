#include <math.h>

#include "hex_to_base64.h"


std::string convertHexCharToBitString(char hexChar) {

	switch (hexChar) {
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
			return "1010";
		case 'B':
			return "1011";
		case 'C':
			return "1100";
		case 'D':
			return "1101";
		case 'E':
			return "1110";
		case 'F':
			return "1111";
		default:
			std::string errMessage = "given char is not hexadecimal char: ";
			errMessage.push_back(hexChar);
			throw std::invalid_argument(errMessage);
	}
}


char convertSixBitStringToBase64Char(std::string sixBitString) {

	if (sixBitString.length() != 6) {
		throw std::invalid_argument("given string is not length 6: " + sixBitString);
	} 

	// convert bit string to decimal value (pow returns double)
	double bitDecimalVal = 0;
	for (int i = 0; i < 6; i++) {
		if (sixBitString[i] == '1') {
			bitDecimalVal += pow(2, 6 - i);
		}
		else if (sixBitString[i] != '0') {
			throw std::invalid_argument("given string is not a bit string: " + sixBitString);
		}
	}

	// map the bit string value to an offset from the first base64 char ASCII val (65: 'A')
	// we could use a big ol' hashmap or switch statement instead but that seems tedious
	int capitalA_ASCII_val = 65;
	int ASCII_offset;
	if (bitDecimalVal <= 25) {
		ASCII_offset = 0;  // capital letters
	}
	else if (bitDecimalVal <= 51) {
		ASCII_offset = 6;  // lower case letters
	}
	else if (bitDecimalVal <= 61) {
		ASCII_offset = -17;  // numbers
	}
	else if (bitDecimalVal < 63) {
		ASCII_offset = -22;  // + 
	}
	else {
		ASCII_offset = -18;  // /
	}

	return capitalA_ASCII_val + ASCII_offset + int(bitDecimalVal);
}
