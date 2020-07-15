#include <iostream>

#include "uint64_bits.h"

#include "gtest/gtest.h"


// cover functionality not exercised by the challenges
namespace {

    TEST(GetBitStringFromHexString_Test, ShortString_succeeds) {

        // should really be separate test cases so one failure doesn't prevent the rest from running
        ASSERT_STREQ(std::string("1001101").c_str(), CustomCrypto::Uint64Bits("4D", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("01001101").c_str(), CustomCrypto::Uint64Bits("4D", "hex", true).GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("1001101").c_str(), CustomCrypto::Uint64Bits("0004D", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("00000000000001001101").c_str(), CustomCrypto::Uint64Bits("0004D", "hex", true).GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("0").c_str(), CustomCrypto::Uint64Bits("0", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("0").c_str(), CustomCrypto::Uint64Bits("0000000000000", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("0000000000000000000000000000000000000000000000000000").c_str(), CustomCrypto::Uint64Bits("0000000000000", "hex", true).GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("1").c_str(), CustomCrypto::Uint64Bits("1", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("10001010110100").c_str(), CustomCrypto::Uint64Bits("22b4", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("0000000000000000000000000010001010110100").c_str(), CustomCrypto::Uint64Bits("00000022b4", "hex", true).GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("101001111110001101010101").c_str(), CustomCrypto::Uint64Bits("a7e355", "hex").GetBitRepresentation().c_str());
        ASSERT_STREQ(std::string("101001111110001101010101").c_str(), CustomCrypto::Uint64Bits("a7e355", "hex", true).GetBitRepresentation().c_str());
    }

    TEST(GetBitStringFromHexString_Test, LongString_succeeds) {
        // should really be separate test cases so one failure doesn't prevent the rest from running
        ASSERT_STREQ(
            std::string("10010010010011101101101001000000110101101101001011011000110110001101001011011100110011100100000011110010110111101110101011100100010000001100010011100100110000101101001011011100010000001101100011010010110101101100101001000000110000100100000011100000110111101101001011100110110111101101110011011110111010101110011001000000110110101110101011100110110100001110010011011110110111101101101").c_str(), 
            CustomCrypto::Uint64Bits("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", "hex").GetBitRepresentation().c_str()
        );
        ASSERT_STREQ(
            std::string("010010010010011101101101001000000110101101101001011011000110110001101001011011100110011100100000011110010110111101110101011100100010000001100010011100100110000101101001011011100010000001101100011010010110101101100101001000000110000100100000011100000110111101101001011100110110111101101110011011110111010101110011001000000110110101110101011100110110100001110010011011110110111101101101").c_str(), 
            CustomCrypto::Uint64Bits("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", "hex", true).GetBitRepresentation().c_str()
        );

        ASSERT_STREQ(
            std::string("11100011000001011000010111001010000000100110011001000110000110100010111110110001101100011011010101110111111000010111110011010111010100010011000010101111011000001111110000111100101110011001001001100110").c_str(), 
            CustomCrypto::Uint64Bits("e30585ca0266461a2fb1b1b577e17cd75130af60fc3cb99266", "hex").GetBitRepresentation().c_str()
        );
        ASSERT_STREQ(
            std::string("00000000000011100011000001011000010111001010000000100110011001000110000110100010111110110001101100011011010101110111111000010111110011010111010100010011000010101111011000001111110000111100101110011001001001100110").c_str(), 
            CustomCrypto::Uint64Bits("000e30585ca0266461a2fb1b1b577e17cd75130af60fc3cb99266", "hex", true).GetBitRepresentation().c_str()
        );

        ASSERT_STREQ(
            std::string("1000000000001111010100000010010000110100101110101010000001111111011101100111011001110").c_str(), 
            CustomCrypto::Uint64Bits("1001ea048697540feecece", "hex").GetBitRepresentation().c_str()
        );
        ASSERT_STREQ(
            std::string("0001000000000001111010100000010010000110100101110101010000001111111011101100111011001110").c_str(), 
            CustomCrypto::Uint64Bits("1001ea048697540feecece", "hex", true).GetBitRepresentation().c_str()
        );
    }

}  // namespace
