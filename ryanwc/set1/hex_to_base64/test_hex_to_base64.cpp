#include <iostream>

#include "hex_to_base64.h"

#include "gtest/gtest.h"


namespace {

    TEST(ConvertHexCharToBitString_Test, IsValidHexChar_succeeds) {
        ASSERT_STREQ(std::string("0000").c_str(), convertHexCharToBitString('0').c_str());
        ASSERT_STREQ(std::string("1111").c_str(), convertHexCharToBitString('F').c_str());
        ASSERT_STREQ(std::string("1111").c_str(), convertHexCharToBitString('f').c_str());
        ASSERT_STREQ(std::string("1010").c_str(), convertHexCharToBitString('A').c_str());
        ASSERT_STREQ(std::string("1010").c_str(), convertHexCharToBitString('a').c_str());
    }

    TEST(ConvertHexCharToBitString_Test, InvalidHexChar_throws) {
        ASSERT_THROW(convertHexCharToBitString('Z'), std::invalid_argument);
        ASSERT_THROW(convertHexCharToBitString('&'), std::invalid_argument);
    }

    TEST(ConvertSixBitStringToBase64Char_Test, CapitalLetters_succeeds) {
        ASSERT_EQ('A', convertSixBitStringToBase64Char("000000"));
        ASSERT_EQ('R', convertSixBitStringToBase64Char("010001"));
        ASSERT_EQ('Z', convertSixBitStringToBase64Char("011001"));
    }

    TEST(ConvertSixBitStringToBase64Char_Test, LowerCaseLetters_succeeds) {
        ASSERT_EQ('a', convertSixBitStringToBase64Char("011010"));
        ASSERT_EQ('n', convertSixBitStringToBase64Char("100111"));
        ASSERT_EQ('z', convertSixBitStringToBase64Char("110011"));
    }

    TEST(ConvertSixBitStringToBase64Char_Test, Numbers_succeeds) {
        ASSERT_EQ('0', convertSixBitStringToBase64Char("110100"));
        ASSERT_EQ('4', convertSixBitStringToBase64Char("111000"));
        ASSERT_EQ('9', convertSixBitStringToBase64Char("111101"));
    }

    TEST(ConvertSixBitStringToBase64Char_Test, PlusSign_succeeds) {
        ASSERT_EQ('+', convertSixBitStringToBase64Char("111110"));
    }

    TEST(ConvertSixBitStringToBase64Char_Test, ForwardSlash_succeeds) {
        ASSERT_EQ('/', convertSixBitStringToBase64Char("111111"));
    }

    TEST(ConvertSixBitStringToBase64Char_Test, InvalidStringLength_throws) {
        ASSERT_THROW(convertSixBitStringToBase64Char("1111111"), std::invalid_argument);
        ASSERT_THROW(convertSixBitStringToBase64Char("11111"), std::invalid_argument);
    }

    TEST(ConvertSixBitStringToBase64Char_Test, NotBitString_throws) {
        ASSERT_THROW(convertSixBitStringToBase64Char("hel|O1"), std::invalid_argument);
        ASSERT_THROW(convertSixBitStringToBase64Char("101200"), std::invalid_argument);
    }

    TEST(ConvertHexxStringToBase64_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("TQ==").c_str(), convertHexStringToBase64("4D").c_str());
    }

    TEST(ConvertHexxStringToBase64_Test, LongString_succeeds) {
        ASSERT_STREQ(
            std::string("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t").c_str(), 
            convertHexStringToBase64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d").c_str()
        );
    }

}  // namespace

