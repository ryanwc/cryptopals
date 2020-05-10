#ifndef HEX_TO_BASE64_H__
#define HEX_TO_BASE64_H__

#include <iostream>


// Convert a hexadecimal string to Base64 string (includes any padding chars)
// e.g. "4D" gives "TQ=="
std::string convertHexStringToBase64(std::string hexString);

#endif // HEX_TO_BASE64_H__
