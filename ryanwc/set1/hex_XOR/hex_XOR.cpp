#include <math.h>
#include <iostream>

#include "../../crypto_lib/uint64_bits.h"
#include "hex_XOR.h"


namespace CustomCrypto {

	std::string XORhexStrings(std::string hexStringOne, std::string hexStringTwo) {

		CustomCrypto::Uint64Bits bitsOne(hexStringOne, "hex");
		CustomCrypto::Uint64Bits bitsTwo(hexStringTwo, "hex");
		CustomCrypto::Uint64Bits xorBits = bitsOne.XOR(bitsTwo);
		return xorBits.GetHexRepresentation();
	}
}