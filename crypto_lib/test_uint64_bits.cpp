#include <iostream>

#include "uint64_bits.h"

#include "gtest/gtest.h"


// cover functionality not exercised by the challenges
namespace {

    TEST(ConvertHexStringToBase64_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("A").c_str(), std::string("B").c_str());
    }

}  // namespace
