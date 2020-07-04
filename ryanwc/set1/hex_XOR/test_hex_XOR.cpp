#include <iostream>

#include "hex_XOR.h"

#include "gtest/gtest.h"


namespace {

    TEST(XORhexStrings_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("0").c_str(), CustomCrypto::XORhexStrings("4D", "4D").c_str());
    }

    TEST(XORhexStrings_Test, LongString_succeeds) {
        ASSERT_STREQ(std::string("746865206b696420646f6e277420706c6179").c_str(), CustomCrypto::XORhexStrings("1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965").c_str());
    }

}  // namespace
