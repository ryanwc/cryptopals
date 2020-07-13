#ifndef HEX_XOR_H__
#define HEX_XOR_H__

#include <iostream>


namespace CustomCrypto {

    // Cryptopals Set 1, Challenge 2
    // Get the hexadecimal string representing XOR of two given hexadecimal strings
    // e.g. "4D, 4D" gives "00" if preserveLeadingZeroes else "0"
    std::string XORhexStrings(std::string hexStringOne, std::string hexStringTwo, bool preserveLeadingZeroes = false);
}

#endif // HEX_XOR_H__
