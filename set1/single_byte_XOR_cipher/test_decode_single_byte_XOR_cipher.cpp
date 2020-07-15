#include <iostream>

#include "decode_single_byte_XOR_cipher.h"

#include "gtest/gtest.h"


namespace {

    TEST(Tautology_Test, Tautology_succeeds) {
        ASSERT_STREQ(std::string("0").c_str(), std::string("0").c_str());
    }

}  // namespace
