#include <math.h>
#include <iostream>
#include <vector>

#include "hex_to_base64.h"


std::string convertHexCharToBitString(char hexChar) {

	switch (hexChar) {
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
		case 'a':
			return "1010";
		case 'B':
		case 'b':
			return "1011";
		case 'C':
		case 'c':
			return "1100";
		case 'D':
		case 'd':
			return "1101";
		case 'E':
		case 'e':
			return "1110";
		case 'F':
		case 'f':
			return "1111";
		default:
			std::string errMessage = "given char is not hexadecimal char: ";
			errMessage.push_back(hexChar);
			throw std::invalid_argument(errMessage);
	}
}


char convertSixBitStringToBase64Char(std::string sixBitString) {

	if (sixBitString.length() != 6) {
		throw std::invalid_argument("given string is not length 6: " + sixBitString);
	} 

	// convert bit string to decimal value (pow returns double)
	double bitDecimalVal = 0;
	for (int i = 0; i < 6; i++) {
		if (sixBitString[i] == '1') {
			bitDecimalVal += pow(2, 5 - i);
		}
		else if (sixBitString[i] != '0') {
			throw std::invalid_argument("given string is not a bit string: " + sixBitString);
		}
	}

	// map the bit string decimal value, which represents its base64 char table index,
	// to its contiguous ASCII group and offset within that group.
	// we could use a big ol' hashmap or switch statement instead but that seems tedious.
	// maybe we could do bitshifting instead of working with bit strings but this overall approach seems OK performance
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

	return ASCII_groupStartVal + groupOffset;
}


std::string convertHexStringToBase64(std::string hexString) {

	if (hexString.length() % 2 != 0) {
		throw std::invalid_argument("purported hex string is not even length: " + hexString);
	}

	// make a bitstring representation of the hex string
	std::string bitString = "";
	for (int i = 0; hexString[i] != '\0'; i++) {
		bitString += convertHexCharToBitString(hexString[i]);
	}

	// convert bitstring to base64 chars with no padding
	std::vector<char> base64Chars;
	while (bitString.length() >= 6) {
		base64Chars.push_back(convertSixBitStringToBase64Char(bitString.substr(0, 6)));
		bitString = bitString.substr(6, bitString.length());
	}

	// deal with padding
	if (bitString.length() > 0) {
		// pad what's there with zeroes to get the final non-pad-char
		while (bitString.length() < 6) {
			bitString.push_back('0');
		}
		base64Chars.push_back(convertSixBitStringToBase64Char(bitString));

		// base64 string with padding should have length multiple of 4
		while (base64Chars.size() % 4 != 0) {
			base64Chars.push_back('=');
		}
	}

	std::string finalBase64String(base64Chars.begin(), base64Chars.end());
	return finalBase64String;
}
