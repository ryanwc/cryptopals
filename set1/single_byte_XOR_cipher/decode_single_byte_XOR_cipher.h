#ifndef DECODE_SINGLE_BYTE_XOR_CIPHER_H__
#define DECODE_SINGLE_BYTE_XOR_CIPHER_H__

#include <iostream>


namespace CustomCrypto {

    std::string DecodeSingleByteXORCipher(std::string hexString, int numThreads);
}

#endif // DECODE_SINGLE_BYTE_XOR_CIPHER_H
