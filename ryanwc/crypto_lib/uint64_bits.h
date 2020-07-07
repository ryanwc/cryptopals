#ifndef UINT64_BITS_H__
#define UINT64_BITS_H__

#include <iostream>
#include <memory>
#include <vector>


namespace CustomCrypto {

    // Represent a source string as bits in a series of uint64_t.
    // Assumes total number of bits represented by given source string fits in int data type.
    // (Dev note: could refactor for factory pattern and/or subclassing, but seems OK for now.)
    class Uint64Bits {

        public:

            Uint64Bits(std::string sourceString, std::string sourceType);
            Uint64Bits(std::unique_ptr<uint64_t> bits, int numBits);

			// Get the total number of bits this Uint64Bits represents
            int GetNumBits() const;

            // Get the total number of Uint64s used for the internal bit representation.
            // Could be more than strictly needed to facilitate, e.g., base64 operations.
            int GetNumUint64s() const;

			// Get a base64 string representation of the bits
            std::string GetBase64Representation();

			// Get a hex string representation of the bits
            std::string GetHexRepresentation();

            // Get a bit string representation of the bits
			std::string GetBitRepresentation();

            // Get a pointer to an array of uint64_t which is a representation of these bits. 
            // Right aligned, so padded with zeroes in most signifcant bits. Use GetNumBits()
            // and GetNumUint64s() to calculate how much padding. It's a copy of the internal
            // representation, so client can modify the return value if desired.
            std::vector<uint64_t> GetBits() const;

            // Get XOR of this with some other bits.
            std::unique_ptr<Uint64Bits> XOR(const Uint64Bits & otherBits);

        private:

            // the source string that this was constructed with
            std::string _sourceString;
            // the type of the source string (e.g. 'hex')
            std::string _sourceType;
            // the hex representation of these bits
            std::string _hexRepresentation;
            // the base64 representaton of these bits
            std::string _base64Representation;
            // the number of bits this represents
            int _numBits;
            // the total number of Uint64s used for the internal bit representation.
            // could be more than strictly needed to facilitate, e.g., base64 operations.
            int _numUint64s;
            // number of padding bits in the "last" uint64_t
            int _numPaddingBits;
            // the internal representation of the bits
            std::vector<uint64_t> _bits;

            // sets the internal bits representation from the source string/type
            void _setBitsFromSource();

            // sets the internal bits representation from the source string (treating as hex)
            void _setBitsFromHex();

            // sets the internal bits representation from the source string (treating as base64)
            void _setBitsFromBase64();

            // convenience func used during translations
            void _setBase64CharFromBits(int bitArrayStartPos, int uintStartPos, int base64Index);

            // set the base64 string representation from internal bits
            void _setBase64StrFromBits();

            // set the hex string representation from internal bits
            void _setHexStrFromBits();

            // get a base64 char from the given int
            char _getBase64CharFromBitVal(uint8_t bitVal);
    };   
}

#endif // UINT64_BITS_H__
