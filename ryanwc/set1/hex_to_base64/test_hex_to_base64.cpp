#include <iostream>

#include "hex_to_base64.h"

#include "gtest/gtest.h"


namespace {

    TEST(ConvertHexCharToBitStringTest, IsValidHexCharSucceeds) {
        ASSERT_STREQ(std::string("0000").c_str(), convertHexCharToBitString('0').c_str());
        ASSERT_STREQ(std::string("1111").c_str(), convertHexCharToBitString('F').c_str());
        ASSERT_STREQ(std::string("1010").c_str(), convertHexCharToBitString('A').c_str());
    }

    TEST(ConvertHexCharToBitStringTest, InvalidHexCharThrows) {
        ASSERT_THROW(convertHexCharToBitString('Z'), std::invalid_argument);
        ASSERT_THROW(convertHexCharToBitString('a'), std::invalid_argument);
        ASSERT_THROW(convertHexCharToBitString('&'), std::invalid_argument);
    }

}  // namespace

