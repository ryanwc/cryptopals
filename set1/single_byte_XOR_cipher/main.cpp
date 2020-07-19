#include <iostream>
#include <chrono>

#include "decode_single_byte_XOR_cipher.h"


int main(int argc, char** argv) {
	std::string hexString;

	if (argc == 2) {
		hexString = argv[1];
	}
	else {
		std::cerr << "Correct usage: [$binary_name $hexString], example: [./decode_single_byte_XOR_cipher 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736]" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		std::cout << CustomCrypto::DecodeSingleByteXORCipher(hexString) << std::endl;
	}
	catch (std::runtime_error err) {
		std::cout << "decoding failed: " << err.what() << std::endl;
		return EXIT_FAILURE;
	}

	// performance measurement
	int currExecution = 0;
	int numExecutions = 10000;
	auto start = std::chrono::high_resolution_clock::now();
	while (currExecution < numExecutions) {
		CustomCrypto::DecodeSingleByteXORCipher("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
		currExecution += 1;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;

	return EXIT_SUCCESS;
}
