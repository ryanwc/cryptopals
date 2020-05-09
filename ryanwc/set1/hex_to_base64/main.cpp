#include <iostream>
#include <vector>

#include "hex_to_base64.h"


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
