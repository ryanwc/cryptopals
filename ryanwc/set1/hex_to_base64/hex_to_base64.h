#ifndef HEX_TO_BASE64_H__
#define HEX_TO_BASE64_H__

#include <iostream>


// Convert a single hexadecimal character to a length-4 bitstring
// e.g. 'A' gives "1010"
std::string convertHexCharToBitString(char hexChar);

// Convert a length-6 bit string to a single base 64 character
// e.g. "011001" gives 'Z'
char convertSixBitStringToBase64Char(std::string sixBitString);

#endif // HEX_TO_BASE64_H__
