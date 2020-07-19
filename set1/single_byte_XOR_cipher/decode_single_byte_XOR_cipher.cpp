#include <math.h>
#include <iostream>
#include <map>

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

	std::string DecodeSingleByteXORCipher(std::string hexString) {

		// note: this could be generalized to work with original plaintext that
		// has punctuation, other languages, etc. scoring could take into account length of ciphertext, 
		// other vars. but we'll just assume English, and use simple alphabet char frequency to score.
		CustomCrypto::Uint64Bits cipherBits(hexString, "hex", true);
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
		char currChar;
		while (true) {
			int startBitsPos = cipherBits.GetNumPaddingBits();
			currStringPos = 0;
			currLikelihood = 100.0;
			for (int i = 0; i < numUint64s; i++) {
				for (; startBitsPos < 64; startBitsPos += 8) { 
					currChar = (rawCipherBits[i] >> (56 - startBitsPos)) | theByte;

					auto likelihoodIter = CryptolibConstants::charLikelihood.find(currChar);
					if (likelihoodIter != CryptolibConstants::charLikelihood.end()) {
						currString[currStringPos] = currChar;
						currLikelihood *= likelihoodIter->second;
						currStringPos += 1;
						continue;
					}
					else if (stringOKifWeSkipChar(currStringPos, currChar, currString)) {
						currString[currStringPos] = currChar;
						// does not factor into likelihood, so this could break if XOR produces a string like
						// "e-e-e-e-e-e" with common letters and puncuation that doesnt violate the simple rules.
						currStringPos += 1;
						continue;
					}
					
					goto endCandidacy;
				}
				startBitsPos = 0;
			}

			if (currLikelihood > bestLikelihood || bestLikelihood < 0) {
				bestLikelihood = currLikelihood;
				swapper = bestString;
				bestString = currString;
				currString = swapper;
			}

		endCandidacy:
			if (theByte == 255) {
				break;
			}
			theByte += 1;
		}


		std::string bestGuess = bestString;
		swapper = NULL;
		free(currString);
		free(bestString);
		return bestGuess;
	}
}