#include <math.h>
#include <iostream>
#include <map>

#include "hex_to_base64.h"


namespace RyanHex {

	// Set the uint64s in bitArray according to hexString
	// e.g. "4D" sets bitArray to array with one uint64 with value "0b00101101[0*56]".
	// Assumes bitArray is correct length
	inline void setBitsFromHex(std::string hexString, uint64_t bitArray[]) {

		int bitArrPos = 0;
		int bitsLeftInPos = 64;

		bitArray[0] = 0b0;

		for (int hexStrPos = 0; hexStrPos < hexString.length(); hexStrPos++) {
			bitArray[bitArrPos] <<= 4;
			switch (hexString[hexStrPos]) {
				case '0':
					bitArray[bitArrPos] |= 0b0000;
					break;
				case '1':
					bitArray[bitArrPos] |= 0b0001;
					break;
				case '2':
					bitArray[bitArrPos] |= 0b0010;
					break;
				case '3':
					bitArray[bitArrPos] |= 0b0011;
					break;
				case '4':
					bitArray[bitArrPos] |= 0b0100;
					break;
				case '5':
					bitArray[bitArrPos] |= 0b0101;
					break;
				case '6':
					bitArray[bitArrPos] |= 0b0110;
					break;
				case '7':
					bitArray[bitArrPos] |= 0b0111;
					break;
				case '8':
					bitArray[bitArrPos] |= 0b1000;
					break;
				case '9':
					bitArray[bitArrPos] |= 0b1001;
					break;
				case 'A':
				case 'a':
					bitArray[bitArrPos] |= 0b1010;
					break;
				case 'B':
				case 'b':
					bitArray[bitArrPos] |= 0b1011;
					break;
				case 'C':
				case 'c':
					bitArray[bitArrPos] |= 0b1100;
					break;
				case 'D':
				case 'd':
					bitArray[bitArrPos] |= 0b1101;
					break;
				case 'E':
				case 'e':
					bitArray[bitArrPos] |= 0b1110;
					break;
				case 'F':
				case 'f':
					bitArray[bitArrPos] |= 0b1111;
					break;
				default:
					std::string errMessage = "given hex string has invalid hexadecimal char: ";
					errMessage.push_back(hexString[hexStrPos]);
					throw std::invalid_argument(errMessage);
			}

			bitsLeftInPos -= 4;
			if (bitsLeftInPos < 1) {
				bitArrPos += 1;
				bitArray[bitArrPos] = 0b0;
				bitsLeftInPos = 64;
			} 
		}
	}


	// Set the base64CharArray[base64Index] according to bitArray[bitArrayStartPos:bitArrayStartPos+6]
	// e.g. if bitArray = {0, 0, 1, 0, 1, 1, 0, 1} and bitArrayStartPos = 0, then base64CharArray[base64Index] set to 'T' (bits "001011")
	// Caller's responsibility to pass correct array/uint start positions
	inline void setBase64CharFromBits(
		uint64_t bitArray[], int bitArrayStartPos, int uintStartPos, char base64CharArray[], int base64Index) {

		// isolate the next bits we care about
		uint64_t firstBitPortion = bitArray[bitArrayStartPos];
		uint8_t bitVal;
		uint8_t intermediate; // doing shifting to clear top bits in one line just preserves original val
		if (uintStartPos > 58) {
			int bitsInFirstPortion = 64 - uintStartPos;
			bitVal = firstBitPortion;
			// slide first portion over left while zeroing most sig bits
			intermediate = bitVal << (8 - bitsInFirstPortion);
			bitVal = intermediate >> 2;  // start first portion at first bits that count for the int val
			int bitsInSecondPortion = 6 - bitsInFirstPortion;
			uint64_t secondBitPortion = bitArray[bitArrayStartPos+1];
			uint8_t secondBitVal = secondBitPortion >> (64 - bitsInSecondPortion);
			// zero top bits in second portion and slide back to OR with first portion
			intermediate = secondBitVal << (8 - bitsInSecondPortion);
			secondBitVal = intermediate >> (8 - bitsInSecondPortion);
			bitVal |= secondBitVal;
		}
		else {
			bitVal = firstBitPortion >> (64 - 6 - uintStartPos);
			// we only care about 6 bits so clear the two most significant bits
			intermediate = bitVal << 2;
			bitVal = intermediate >> 2;
		}

		// map the decimal value, which represents its base64 char table index, to its contiguous ASCII group and offset within that group.
		// we could use a big ol' hashmap or switch statement instead but that seems tedious.
		// also, using a static mapping array like `base64CharArray[base64Index] = base64CharTable[int(bitDecimalVal)]` was actually a bit slower
		int ASCII_groupStartVal;
		int groupOffset;
		if (bitVal < 26) {
			ASCII_groupStartVal = 65;  // capital letters
			groupOffset = bitVal;
		}
		else if (bitVal < 52) {
			ASCII_groupStartVal = 97;  // lower case letters
			groupOffset = bitVal - 26;
		}
		else if (bitVal < 62) {
			ASCII_groupStartVal = 48;  // numbers
			groupOffset = bitVal - 52;
		}
		else if (bitVal < 63) {
			ASCII_groupStartVal = 43;  // + 
			groupOffset = 0;
		}
		else {
			ASCII_groupStartVal = 47;  // /
			groupOffset = 0;
		}

		base64CharArray[base64Index] = ASCII_groupStartVal + groupOffset;
	}


	// assumes total number of bits represented by given hex strings fits in int data type
	std::string convertHexStringToBase64(std::string hexString) {

		int hexStringLen = hexString.length();

		if (hexStringLen % 2 != 0) {
			throw std::invalid_argument("purported hex string is not even length: " + hexString);
		}

		int totalBits = hexString.length() * 4;
		int numSetBitsInLast64 = totalBits % 64;
		int uint64ArrayLen = int(ceil(totalBits / 64.0)) + 1;
		uint64_t uint64Array[uint64ArrayLen];

		setBitsFromHex(hexString, uint64Array);

		// ensure bits we care about in last uint64 are most significant
		uint64Array[uint64ArrayLen-2] <<= 64 - numSetBitsInLast64;
		// put trailing 0s so we can use same function for regular conversion as well as padding
		uint64Array[uint64ArrayLen-1] = 0b0;

		int numBase64Chars = totalBits / 6;
		int base64StrLen = numBase64Chars;
		if (totalBits % 6 != 0) {
			base64StrLen += 1;
			while (base64StrLen % 4 != 0) {
				base64StrLen += 1;
			}
		}

		char base64CharArray[base64StrLen+1];  // +1 for null char string terminator
		int numBitsConverted;
		int uint64ArrIndex = 0;
		int base64CharIndex = 0;
		int uintBitStartPos = 0;
		for (numBitsConverted = 0; numBitsConverted < totalBits; numBitsConverted += 6) {
			setBase64CharFromBits(uint64Array, uint64ArrIndex, uintBitStartPos, base64CharArray, base64CharIndex);
			base64CharIndex += 1;
			uintBitStartPos += 6;
			if (uintBitStartPos >= 64) {
				// could also do mod but is mod arithmetic slower?
				uintBitStartPos -= 64;
				uint64ArrIndex += 1;
			}
		}

		while (base64CharIndex < base64StrLen) {
			base64CharArray[base64CharIndex] = '=';  // base64 padding char padding
			base64CharIndex += 1;
		}

		base64CharArray[base64CharIndex] = '\0';

		std::string finalBase64String(base64CharArray);
		return finalBase64String;
	}

}
