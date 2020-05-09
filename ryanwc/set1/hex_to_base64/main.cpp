#include <iostream>
#include <vector>
#include <math.h>


// Convert a single hexadecimal character to a length-4 bitstring
// e.g. 'A' gives "1010"
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
			std::string errMessage = "expect hexadecimal char but got: ";
			errMessage.push_back(hexChar);
			throw std::invalid_argument(errMessage);
	}
}


// Conver a length-6 bit string to a single base 64 character
// e.g. "011001" gives 'r'
char convertSixBitStringToBase64Char(std::string sixBitString) {

	double bitVal = 0;
	for (int i = 0; i < 6; i++) {
		if (sixBitString[i] == '1') {
			bitVal += pow(2, 6 - i);
		}
	}

	int capitalA_ASCII_val = 65;
	int offset;
	if (bitVal <= 25) {
		offset = 0;  // capital letters
	}
	else if (bitVal <= 51) {
		offset = 6;  // lower case letters
	}
	else if (bitVal <= 61) {
		offset = -17;  // numbers
	}
	else if (bitVal == 62) {
		offset = -22;  // + 
	}
	else {
		offset = -18;  // /
	}

	return capitalA_ASCII_val + offset + int(bitVal);
}



int main(int argc, char** argv) {
	char* hexString;

	if (argc == 2) {
		hexString = argv[1];
	}
	else {
		std::cerr << "Correct usage: [$binary_name $hexString], example: [./hex_to_base64 4D2]" << std::endl;
		return EXIT_FAILURE;
	}

	std::string bitString = "";
	std::vector<char> base64Vector;

	// make a bitstring representation of the hex string
	for (int i = 0; hexString[i] != '\0'; i++) {
		bitString += convertHexCharToBitString(hexString[i]);
	}

	std::cout << bitString << std::endl;

	return EXIT_SUCCESS;
}
