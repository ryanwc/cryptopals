#include <math.h>
#include <iostream>

#include "../../crypto_lib/uint64_bits.h"
#include "hex_XOR.h"


namespace CustomCrypto {

	std::string XORhexStrings(std::string hexStringOne, std::string hexStringTwo, bool preserveLeadingZeroes) {

		CustomCrypto::Uint64Bits bitsOne(hexStringOne, "hex", preserveLeadingZeroes);
		CustomCrypto::Uint64Bits bitsTwo(hexStringTwo, "hex", preserveLeadingZeroes);
		std::unique_ptr<CustomCrypto::Uint64Bits> xorBits = bitsOne.XOR(bitsTwo, preserveLeadingZeroes);
		return xorBits->GetHexRepresentation();
	}
}