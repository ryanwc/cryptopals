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

	std::cout << convertHexStringToBase64(hexString) << std::endl;

	return EXIT_SUCCESS;
}
