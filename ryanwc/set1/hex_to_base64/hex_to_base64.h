#ifndef HEX_TO_BASE64_H__
#define HEX_TO_BASE64_H__

#include <iostream>


// Convert a single hexadecimal character to a length-4 bitstring
// e.g. 'A' gives "1010"
std::string convertHexCharToBitString(char hexChar);

// Convert a length-6 bit string to a single base 64 character
// e.g. "011001" gives 'Z'
char convertSixBitStringToBase64Char(std::string sixBitString);

// Convert a hexadecimal string to Base64 string (includes any padding chars)
// e.g. "4D" gives "TQ=="
std::string convertHexStringToBase64(std::string hexString);

#endif // HEX_TO_BASE64_H__
