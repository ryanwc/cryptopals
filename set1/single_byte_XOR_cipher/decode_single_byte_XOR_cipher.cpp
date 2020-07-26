#include <math.h>
#include <iostream>
#include <map>
#include <thread>
#include <tuple>

#include "../../threading/results_pool.h"
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

	/*
	 * Worker func. Returns the likelihood/string combo of decoding the given encoded bits with the given byte.
	 */
	std::tuple<long double, std::string> GetLikelihoodOfByteCipher(
			uint64_t* encodedBits, int numUint64s, uint8_t theByte, int startPadding, int numChars) {

		// note: this could be generalized to work with original plaintext that
		// has more punctuation, other languages, etc. scoring could take into account length of ciphertext, 
		// 2/3/etc letter tuple frequencies, other vars, you name it, etc.
		// but we'll just assume English, and use simple alphabet char frequency to score.

		char* decodedString = (char*) malloc(sizeof(char) * numChars + 1);
		decodedString[numChars] = '\0';
		long double logLikelihood = 1.0;
		int currStringPos = 0;
		char currChar;
		int startBitsPos = startPadding;
		// iterate the bits in this uint64_t, inspecting one char (from XORing with the byte) at a time
		for (int i = 0; i < numUint64s; i++) {
			for (; startBitsPos < 64; startBitsPos += 8) { 
				currChar = (encodedBits[numUint64s] >> (56 - startBitsPos)) ^ theByte;
				decodedString[currStringPos] = currChar;

				auto likelihoodIter = CryptolibConstants::charLogLikelihood.find(currChar);
				if (likelihoodIter != CryptolibConstants::charLogLikelihood.end()) {
					logLikelihood += likelihoodIter->second;
					currStringPos += 1;
					continue;
				}
				else if (stringOKifWeSkipChar(currStringPos, currChar, decodedString)) {
					// does not factor into likelihood, so this could break if XOR produces a string like
					// "e-e e-e-e.e" with common letters and puncuation/spacing that doesnt violate the simple rules.
					currStringPos += 1;
					continue;
				}
				// it wasn't a char we know how to deal with -- throw this string out
				goto doneDecoding;
			}
			startBitsPos = 0;
		}

	doneDecoding:
		std::string theGuess = decodedString;
		free(decodedString);
		return std::tuple<long double, std::string>(logLikelihood, theGuess);
	}

	std::string DecodeSingleByteXORCipher(std::string hexString, int numThreads) {

		CustomThreading::ResultsPool<decltype(GetLikelihoodOfByteCipher)> pool(numThreads, &GetLikelihoodOfByteCipher);

		// could also pass in ref to result array instead of returning tuple from result func 
		// but let's try out the results-passing feature of ResultsPool
		// or could do something like "pass in one byte at a time, free workers do it and sync on 'best' vars"
		CustomCrypto::Uint64Bits cipherBits(hexString, "hex", true);
		int numUint64s = cipherBits.GetNumUint64s();
		int numChars = cipherBits.GetNumBits() / 8;
		int startPadding  = cipherBits.GetNumPaddingBits();
		std::unique_ptr<uint64_t[]> rawCipherBits = cipherBits.GetBits();
		uint64_t* bitsPtr = rawCipherBits.get();
		uint8_t theByte = 0;
		while (true) {
			pool.SubmitWork(bitsPtr, numUint64s, theByte, startPadding, numChars);
			if (theByte == 255) { break; }
			theByte += 1;
		}

		long double bestLogLikelihood = 1.0;
		long double currLogLikelihood;
		std::string bestGuess = "";
		auto bestGuesses = pool.WaitForAllResults();

		for (int i = 0; i < bestGuesses.size(); i++) {
			currLogLikelihood = std::get<0>(bestGuesses[i]);
			if (currLogLikelihood < 0 && currLogLikelihood > bestLogLikelihood) {
				bestLogLikelihood = currLogLikelihood;
				bestGuess = std::get<1>(bestGuesses[i]);
			}
		}

		if (bestLogLikelihood > 0) {
			throw std::runtime_error("could not find any likely decoded strings using English alphabet with minimal puncuation");
		}

		return bestGuess;
	}
}