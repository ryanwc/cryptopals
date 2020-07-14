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

}  // namespace
