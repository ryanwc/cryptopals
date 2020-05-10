#include <math.h>
#include <iostream>

#include "hex_to_base64.h"


// Set the bools in bitArray according to hexString
// e.g. "4D" sets bitArray to {0, 0, 1, 0, 1, 1, 0, 1}
// Assumes bitArray is correct length
inline void setBitsFromHex(std::string hexString, bool bitArray[]) {

	int bitArrPos = 0;
	for (int hexStrPos = 0; hexStrPos < hexString.length(); hexStrPos++) {
		switch (hexString[hexStrPos]) {
			case '0':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 0;
				break;
			case '1':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 1;
				break;
			case '2':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 0;
				break;
			case '3':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 1;
				break;
			case '4':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 0;
				break;
			case '5':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 1;
				break;
			case '6':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 0;
				break;
			case '7':
				bitArray[bitArrPos] = 0;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 1;
				break;
			case '8':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 0;
				break;
			case '9':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 1;
				break;
			case 'A':
			case 'a':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 0;
				break;
			case 'B':
			case 'b':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 0;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 1;
				break;
			case 'C':
			case 'c':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 0;
				break;
			case 'D':
			case 'd':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 0;
				bitArray[bitArrPos+3] = 1;
				break;
			case 'E':
			case 'e':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 0;
				break;
			case 'F':
			case 'f':
				bitArray[bitArrPos] = 1;
				bitArray[bitArrPos+1] = 1;
				bitArray[bitArrPos+2] = 1;
				bitArray[bitArrPos+3] = 1;
				break;
			default:
				std::string errMessage = "given hex string has invalid hexadecimal char: ";
				errMessage.push_back(hexString[hexStrPos]);
				throw std::invalid_argument(errMessage);
		}

		bitArrPos += 4;
	}
}


// Set the base64CharArray[base64Index] according to bitArray[bitArrayStartPos:bitArrayStartPos+6]
// e.g. if bitArray = {0, 0, 1, 0, 1, 1, 0, 1} and bitArrayStartPos = 0, then base64CharArray[base64Index] set to 'T' (bits "001011")
inline void setBase64CharFromBits(bool bitArray[], int bitArrayStartPos, char base64CharArray[], int base64Index) {

	// convert bits to decimal value (pow returns double)
	// maybe we could do bitshifting instead of working with bools but this overall approach seems OK performance.
	double bitDecimalVal = 0;
	for (int i = 0; i < 6; i++) {
		if (bitArray[bitArrayStartPos + i] == 1) {
			bitDecimalVal += pow(2, 5 - i);
		}
		else if (bitArray[bitArrayStartPos + i] != 0) {
			throw std::invalid_argument("bit array is not bits");
		}
	}

	// map the decimal value, which represents its base64 char table index, to its contiguous ASCII group and offset within that group.
	// we could use a big ol' hashmap or switch statement instead but that seems tedious.
	// also, using a static mapping array like `base64CharArray[base64Index] = base64CharTable[int(bitDecimalVal)]` was actually a bit slower
	int ASCII_groupStartVal;
	int groupOffset;
	int bitIntVal = int(bitDecimalVal);
	if (bitDecimalVal < 26) {
		ASCII_groupStartVal = 65;  // capital letters
		groupOffset = bitIntVal;
	}
	else if (bitDecimalVal < 52) {
		ASCII_groupStartVal = 97;  // lower case letters
		groupOffset = bitIntVal - 26;
	}
	else if (bitDecimalVal < 62) {
		ASCII_groupStartVal = 48;  // numbers
		groupOffset = bitIntVal - 52;
	}
	else if (bitDecimalVal < 63) {
		ASCII_groupStartVal = 43;  // + 
		groupOffset = 0;
	}
	else {
		ASCII_groupStartVal = 47;  // /
		groupOffset = 0;
	}

	base64CharArray[base64Index] = ASCII_groupStartVal + groupOffset;
}


std::string convertHexStringToBase64(std::string hexString) {

	int hexStringLen = hexString.length();

	if (hexStringLen % 2 != 0) {
		throw std::invalid_argument("purported hex string is not even length: " + hexString);
	}

	int bitArrayLen = hexStringLen*4;
	bool bitArray[bitArrayLen];
	setBitsFromHex(hexString, bitArray);

	int base64StrLen = bitArrayLen / 6;
	if (bitArrayLen % 6 != 0) {
		base64StrLen += 1;
		while (base64StrLen % 4 != 0) {
			base64StrLen += 1;
		}
	}

	char base64CharArray[base64StrLen+1];  // +1 for null char string terminator
	int bitIndex;
	int base64CharIndex = 0;
	for (bitIndex = 0; bitIndex < bitArrayLen - 6; bitIndex += 6) {
		setBase64CharFromBits(bitArray, bitIndex, base64CharArray, base64CharIndex);
		base64CharIndex += 1;
	}

	// deal with padding
	if (bitIndex < bitArrayLen) {
		// pad what's there with zeroes to get the final non-pad-char
		bool finalCharBitArray[] = {0, 0, 0, 0, 0, 0};
		int padCharIndex = 0;
		while (bitIndex < bitArrayLen) {
			finalCharBitArray[padCharIndex] = bitArray[bitIndex];
			bitIndex += 1;
			padCharIndex += 1;
		}

		setBase64CharFromBits(finalCharBitArray, 0, base64CharArray, base64CharIndex);
		base64CharIndex += 1;

		while (base64CharIndex < base64StrLen) {
			base64CharArray[base64CharIndex] = '=';
			base64CharIndex += 1;
		}
	}

	base64CharArray[base64CharIndex] = '\0';

	std::string finalBase64String(base64CharArray);
	return finalBase64String;
}
