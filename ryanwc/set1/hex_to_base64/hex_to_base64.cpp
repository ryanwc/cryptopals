#include <math.h>
#include <iostream>
#include <map>

#include "hex_to_base64.h"
#include "../../crypto_lib/uint64_bits.h"


namespace CustomCrypto {

	std::string ConvertHexStringToBase64(std::string hexString, bool preserveLeadingZeroes) {

		CustomCrypto::Uint64Bits theBits(hexString, "hex", preserveLeadingZeroes);
		return theBits.GetBase64Representation();
	}
}
