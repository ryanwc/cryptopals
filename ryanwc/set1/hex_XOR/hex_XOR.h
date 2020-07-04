#ifndef HEX_XOR_H__
#define HEX_XOR_H__

#include <iostream>


namespace CustomCrypto {

    // Get the hexadecimal string representing XOR of two given hexadecimal strings
    // e.g. "4D, 4D" gives "0"
    std::string XORhexStrings(std::string hexStringOne, std::string hexStringTwo);
}

#endif // HEX_XOR_H__
