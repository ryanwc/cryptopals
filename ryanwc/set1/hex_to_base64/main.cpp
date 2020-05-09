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

	try {
		std::cout << convertHexStringToBase64(hexString) << std::endl;
	}
	catch (std::invalid_argument err) {
		std::cout << "conversion failed: " << err.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
