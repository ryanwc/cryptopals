#include <iostream>

#include "hex_to_base64.h"

#include "gtest/gtest.h"


namespace {

    TEST(ConvertHexStringToBase64_Test, ShortString_succeeds) {
        ASSERT_STREQ(std::string("AA==").c_str(), CustomCrypto::ConvertHexStringToBase64("0").c_str());
        ASSERT_STREQ(std::string("AA==").c_str(), CustomCrypto::ConvertHexStringToBase64("00000000").c_str());
        ASSERT_STREQ(std::string("AQ==").c_str(), CustomCrypto::ConvertHexStringToBase64("1").c_str());
        ASSERT_STREQ(std::string("AQ==").c_str(), CustomCrypto::ConvertHexStringToBase64("01").c_str());
        ASSERT_STREQ(std::string("AQ==").c_str(), CustomCrypto::ConvertHexStringToBase64("00001").c_str());
        ASSERT_STREQ(std::string("TQ==").c_str(), CustomCrypto::ConvertHexStringToBase64("4D").c_str());
        ASSERT_STREQ(std::string("lj0=").c_str(), CustomCrypto::ConvertHexStringToBase64("963d").c_str());
        ASSERT_STREQ(std::string("IrQ=").c_str(), CustomCrypto::ConvertHexStringToBase64("22b4").c_str());
        ASSERT_STREQ(std::string("p+NV").c_str(), CustomCrypto::ConvertHexStringToBase64("a7e355").c_str());
        ASSERT_STREQ(std::string("SKvW").c_str(), CustomCrypto::ConvertHexStringToBase64("48abd6").c_str());
    }

    TEST(ConvertHexStringToBase64_Test, HandlesMixedCaps_succeeds) {
        ASSERT_STREQ(std::string("SKvW").c_str(), CustomCrypto::ConvertHexStringToBase64("48aBd6").c_str());
    }

    TEST(ConvertHexStringToBase64_Test, LongString_succeeds) {
        ASSERT_STREQ(
            std::string("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d").c_str()
        );
        ASSERT_STREQ(
            std::string("sCZKTLZIAI4=").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("b0264a4cb648008e").c_str()
        );
        ASSERT_STREQ(
            std::string("QLQvSN5cNZA=").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("40b42f48de5c3590").c_str()
        );
        ASSERT_STREQ(
            std::string("mTBIVHClfyW3zCU=").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("9930485470a57f25b7cc25").c_str()
        );
        ASSERT_STREQ(
            std::string("EAHqBIaXVA/uzs4=").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("1001ea048697540feecece").c_str()
        );
        ASSERT_STREQ(
            std::string("4wWFygJmRhovsbG1d+F811Ewr2D8PLmSZg==").c_str(), 
            CustomCrypto::ConvertHexStringToBase64("e30585ca0266461a2fb1b1b577e17cd75130af60fc3cb99266").c_str()
        );
    }

    TEST(ConvertHexStringToBase64_Test, InvalidHexChar_throws) {
        ASSERT_THROW(CustomCrypto::ConvertHexStringToBase64("4Z"), std::invalid_argument);
        ASSERT_THROW(CustomCrypto::ConvertHexStringToBase64("4&"), std::invalid_argument);
        ASSERT_THROW(CustomCrypto::ConvertHexStringToBase64("e30585ca0266461a2f*1b1b577e17cd75130af60fc3cb99266"), std::invalid_argument);
    }

}  // namespace
