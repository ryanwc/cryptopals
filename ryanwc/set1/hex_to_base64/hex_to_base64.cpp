#include <math.h>
#include <iostream>

#include "hex_to_base64.h"


inline void setBitsFromHex(std::string hexString, bool bitCharArray[]) {

	int bitArrPos = 0;
	for (int hexStrPos = 0; hexStrPos < hexString.length(); hexStrPos++) {
		switch (hexString[hexStrPos]) {
			case '0':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case '1':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case '2':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case '3':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case '4':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case '5':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case '6':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case '7':
				bitCharArray[bitArrPos] = 0;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case '8':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case '9':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case 'A':
			case 'a':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case 'B':
			case 'b':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 0;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case 'C':
			case 'c':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case 'D':
			case 'd':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 0;
				bitCharArray[bitArrPos+3] = 1;
				break;
			case 'E':
			case 'e':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 0;
				break;
			case 'F':
			case 'f':
				bitCharArray[bitArrPos] = 1;
				bitCharArray[bitArrPos+1] = 1;
				bitCharArray[bitArrPos+2] = 1;
				bitCharArray[bitArrPos+3] = 1;
				break;
			default:
				std::string errMessage = "given hex string has invalid hexadecimal char: ";
				errMessage.push_back(hexString[hexStrPos]);
				throw std::invalid_argument(errMessage);
		}

		bitArrPos += 4;
	}
}


inline void setBase64CharFromBitArray(bool bitArray[], int bitArrayStartPos, char base64CharArray[], int base64Index) {

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
	// also, using an array like `return base64CharArray[int(bitDecimalVal)]` was actually a bit slower than this
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
		setBase64CharFromBitArray(bitArray, bitIndex, base64CharArray, base64CharIndex);
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

		setBase64CharFromBitArray(finalCharBitArray, 0, base64CharArray, base64CharIndex);
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
