#include <math.h>
#include <iostream>
#include <map>
#include <thread>

#include "../../crypto_lib/uint64_bits.h"
#include "../../crypto_lib/cryptolib_constants.h"
#include "decode_single_byte_XOR_cipher.h"


namespace CustomCrypto {

	inline bool stringOKifWeSkipChar(int currStringPos, char currChar, char* currString) {
		// we're only handling select non-english alphabet chars and even then, 
		// two in a row only if this is space and last one not space. so this is pretty limited.
		if (currStringPos > 0) {
			bool isSkipChar = CryptolibConstants::skipChar.find(currChar) != CryptolibConstants::skipChar.end();
			if (isSkipChar) {
				bool lastWasSkip = CryptolibConstants::skipChar.find(currString[currStringPos-1]) != CryptolibConstants::skipChar.end();
				if (!lastWasSkip) {
					return true;  // dont care about anything else, just skip
				}
				// now have to check more rules about multiple skip chars in a row
				// can only have space after another skip char
				if (currChar != ' ') {
					return false;
				}
				// prevent multiple spaces in a row
				bool lastWasSpace = currString[currStringPos-1] == ' ';
				if (lastWasSpace) {
					return false;
				}

				return true;  // it's a skip char and we didn't violate any rules
			}
		}
		return false;
	}

	std::string DecodeSingleByteXORCipher(std::string hexString, int numThreads) {

		std::cout << "using " << numThreads << " threads" << std::endl;

		// note: this could be generalized to work with original plaintext that
		// has more punctuation, other languages, etc. scoring could take into account length of ciphertext, 
		// 2/3/etc letter tuple frequencies, other vars, you name it, etc.
		// but we'll just assume English, and use simple alphabet char frequency to score.
		CustomCrypto::Uint64Bits cipherBits(hexString, "hex", true);
		int numUint64s = cipherBits.GetNumUint64s();
		int numChars = cipherBits.GetNumBits() / 8;
		std::unique_ptr<uint64_t[]> rawCipherBits = cipherBits.GetBits();

		// todo: use threadpool and compare performance
		char* currString = (char*) malloc(sizeof(char) * numChars + 1);
		char* bestString = (char*) malloc(sizeof(char) * numChars + 1);
		currString[numChars] = '\0';
		bestString[numChars] = '\0';
		char* swapper = NULL;
		long double currLogLikelihood = 0.0;
		long double bestLogLikelihood = 1.0;
		uint8_t theByte = 0;
		int startBitsPos;
		int currStringPos;
		char currChar;
		while (true) {
			// reset for new candidate string, to built from XORing given bits with 'next' byte
			int startBitsPos = cipherBits.GetNumPaddingBits();
			currStringPos = 0;
			currLogLikelihood = 0.0;
			// iterate the bits array
			for (int i = 0; i < numUint64s; i++) {
				// iterate the bits in this uint64_t, inspecting one char (from XORing with the byte) at a time
				for (; startBitsPos < 64; startBitsPos += 8) { 
					currChar = (rawCipherBits[i] >> (56 - startBitsPos)) ^ theByte;
					currString[currStringPos] = currChar;

					auto likelihoodIter = CryptolibConstants::charLogLikelihood.find(currChar);
					if (likelihoodIter != CryptolibConstants::charLogLikelihood.end()) {
						currLogLikelihood += likelihoodIter->second;
						currStringPos += 1;
						continue;
					}
					else if (stringOKifWeSkipChar(currStringPos, currChar, currString)) {
						// does not factor into likelihood, so this could break if XOR produces a string like
						// "e-e e-e-e.e" with common letters and puncuation/spacing that doesnt violate the simple rules.
						currStringPos += 1;
						continue;
					}
					// it wasn't a char we know how to deal with -- throw this string out
					goto endCandidacy;
				}
				startBitsPos = 0;
			}

			if (currLogLikelihood > bestLogLikelihood || bestLogLikelihood > 0) {
				//std::cout << "found better likelihood: " << currLogLikelihood << " for string \"" << currString << "\"" << std::endl;
				bestLogLikelihood = currLogLikelihood;
				swapper = bestString;
				bestString = currString;
				currString = swapper;
			}

		endCandidacy:
			if (theByte == 255) {
				break;  // doing break in while(true) prevents dealing with uint8_t overflow
			}
			theByte += 1;
		}


		std::string bestGuess = bestString;
		swapper = NULL;
		free(currString);
		free(bestString);

		if (bestLogLikelihood > 0) {
			throw std::runtime_error("could not find any likely decoded strings using English alphabet with minimal puncuation");
		}

		return bestGuess;
	}
}