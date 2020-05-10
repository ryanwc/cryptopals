#include <iostream>

#include "hex_to_base64.h"


int main(int argc, char** argv) {
	std::string hexString;

	if (argc == 2) {
		hexString = argv[1];
	}
	else {
		std::cerr << "Correct usage: [$binary_name $hexString], example: [./hex_to_base64 4D]" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		std::cout << convertHexStringToBase64(hexString) << std::endl;
	}
	catch (std::invalid_argument err) {
		std::cout << "conversion failed: " << err.what() << std::endl;
		return EXIT_FAILURE;
	}

	/* performance measurement
	int currExecution = 0;
	int numExecutions = 10000;
	auto start = std::chrono::high_resolution_clock::now();
	while (currExecution < numExecutions) {
		convertHexStringToBase64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
		currExecution += 1;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
	*/

	return EXIT_SUCCESS;
}
