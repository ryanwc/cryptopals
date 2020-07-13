#include <iostream>

#include "hex_XOR.h"

#include "gtest/gtest.h"


namespace {

    TEST(XORhexStrings_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("0").c_str(), CustomCrypto::XORhexStrings("4D", "4D").c_str());
        ASSERT_STREQ(std::string("0").c_str(), CustomCrypto::XORhexStrings("04D", "04D").c_str());
        ASSERT_STREQ(std::string("000").c_str(), CustomCrypto::XORhexStrings("04D", "04D", true).c_str());
    }

    TEST(XORhexStrings_Test, LongString_succeeds) {
        ASSERT_STREQ(std::string("746865206B696420646F6E277420706C6179").c_str(), CustomCrypto::XORhexStrings("1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965").c_str());
    }

}  // namespace
