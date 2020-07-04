#ifndef HEX_TO_BASE64_H__
#define HEX_TO_BASE64_H__

#include <iostream>


namespace CustomCrypto {

    // Convert a hexadecimal string to Base64 string (includes any padding chars)
    // e.g. "4D" gives "TQ=="
    std::string convertHexStringToBase64(std::string hexString);

	// Set the uint64s in bitArray according to hexString
	// e.g. "4D" sets bitArray to array with one uint64 with value "0b00101101[0*56]".
	// Assumes bitArray is correct length
	void setBitsFromHex(std::string hexString, uint64_t bitArray[]);
}

#endif // HEX_TO_BASE64_H__
