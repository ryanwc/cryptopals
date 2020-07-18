#include <math.h>
#include <iostream>

#include "../../crypto_lib/uint64_bits.h"
#include "decode_single_byte_XOR_cipher.h"


namespace CustomCrypto {

	std::string DecodeSingleByteXORCipher(std::string hexString) {

		// note: this could be generalized to work with original plaintext that
		// has punctuation, other languages, etc. scoring could take into account length of ciphertext, 
		// other vars. but we'll just assume English, and use simple alphabet char frequency to score.

		CustomCrypto::Uint64Bits cipherBits(hexString, "hex");
		int numUint64s = cipherBits.GetNumUint64s();
		int numChars = cipherBits.GetNumBits() / 8;
		std::unique_ptr<uint64_t[]> rawCipherBits = cipherBits.GetBits();

		// todo: use threadpool and compare performance
		char* currString = (char*) malloc(sizeof(char) * numChars + 1);
		double currLikelihood = 0.0;  // probably want log likelihood or else will get too small
		currString[numChars] = '\0';
		char* bestString = (char*) malloc(sizeof(char) * numChars + 1);
		char* swapper = NULL;
		double bestLikelihood = -1.0;
		bestString[numChars] = '\0';
		uint8_t theByte = 0;
		int startBitsPos;
		int currStringPos;
		while (true) {
			int startBitsPos = cipherBits.GetNumPaddingBits();
			currStringPos = 0;
			for (int i = 0; i < numUint64s; i++) {
				for (; startBitsPos < 64; startBitsPos += 8) { 
					currString[i] = (rawCipherBits[i] >> (55 - startBitsPos)) | theByte;
					// currLikelihood *= char likelihood
					currStringPos += 1;
				}
				startBitsPos = 0;
			}

			if (currLikelihood > bestLikelihood || bestLikelihood < 0) {
				bestLikelihood = currLikelihood;
				swapper = bestString;
				bestString = currString;
				currString = swapper;
			}

			// iterate the bytes in that XOR'ing to make new bits, putting into string
			// score by multiplying total probability of string given each char

			if (theByte == 255) {
				break;
			}
			theByte += 1;
		}


		// not right yet
		swapper = NULL;
		if (bestString) {
			
		}
		
		// return best candidate string. or possibility a selection of best.
		return  "";
	}
}