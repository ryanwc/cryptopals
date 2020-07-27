#include <iostream>

#include "decode_single_byte_XOR_cipher.h"

#include "gtest/gtest.h"


namespace {

    TEST(Cryptopals_Test, TheirExample_succeeds) {
        ASSERT_STREQ(std::string("Cooking MC's like a pound of bacon").c_str(), 
                     CustomCrypto::DecodeSingleByteXORCipher("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736", 4).c_str()
        );
    }

}  // namespace
