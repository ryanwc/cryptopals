#include <iostream>

#include "hex_to_base64.h"

#include "gtest/gtest.h"


namespace {

    TEST(ConvertHexCharToBitStringTest, IsValidHexCharSucceeds) {
        ASSERT_STREQ(std::string("0000").c_str(), convertHexCharToBitString('0').c_str());
    }

}  // namespace

