#include <iostream>

#include "base64_XOR.h"

#include "gtest/gtest.h"


namespace {

    TEST(ConvertHexStringToBase64_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("0").c_str(), XORbase64HexStrings("4D", "4D").c_str());
    }

}  // namespace
