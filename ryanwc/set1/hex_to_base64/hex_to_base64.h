#ifndef HEX_TO_BASE64_H__
#define HEX_TO_BASE64_H__

#include <iostream>


// Set the bools in bitArray according to hexString
// e.g. "4D" sets bitArray to {0, 0, 1, 0, 1, 1, 0, 1}
// Assumes bitArray is correct length
void setBitsFromHex(std::string hexString, bool bitArray[]);

// Set the base64CharArray[base64Index] according to bitArray[bitArrayStartPos:bitArrayStartPos+6]
// e.g. if bitArray = {0, 0, 1, 0, 1, 1, 0, 1} and bitArrayStartPos = 0, then base64CharArray[base64Index] set to 'T' (bits "001011")
void setBase64CharFromBits(bool bitArray[], int bitArrayStartPos, char base64CharArray[], int base64Index);

// Convert a hexadecimal string to Base64 string (includes any padding chars)
// e.g. "4D" gives "TQ=="
std::string convertHexStringToBase64(std::string hexString);

#endif // HEX_TO_BASE64_H__
