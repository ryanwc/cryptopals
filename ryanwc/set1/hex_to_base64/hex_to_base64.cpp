#include <math.h>
#include <iostream>
#include <map>

#include "hex_to_base64.h"
#include "uint64_bits.h"


namespace CustomCrypto {

	std::string ConvertHexStringToBase64(std::string hexString) {

		CustomCrypto::Uint64Bits theBits(hexString, "hex");
		return theBits.GetBase64Representation();
	}
}
