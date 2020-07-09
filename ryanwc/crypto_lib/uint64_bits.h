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
            Uint64Bits(std::unique_ptr<uint64_t[]> bits, int numBits, int numUint64s, int numPaddingBits);
            ~Uint64Bits();

			// Get the total number of bits this Uint64Bits represents
            int GetNumBits() const;

            // number of padding bits in the "last" uint64_t
            int GetNumPaddingBits() const;

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
            // representation, client is sole owner.
            std::unique_ptr<uint64_t[]> GetBits() const;

            // Get XOR of this with some other bits.
            std::unique_ptr<Uint64Bits> XOR(const Uint64Bits & otherBits);

        private:

            // the hex representation of these bits
            char* _hexRepresentation;
            // the base64 representaton of these bits
            char* _base64Representation;
            // the bit string representation of these bits
            char* _bitRepresentation;
            // the number of bits this represents
            int _numBits;
            // the total number of Uint64s used for the internal bit representation.
            // could be more than strictly needed to facilitate, e.g., base64 operations.
            int _numUint64s;
            // number of padding bits in the "last" uint64_t
            int _numPaddingBits;
            // the internal representation of the bits
            uint64_t* _bits;

            // sets internal vars from source string/type
            void _initInternalsFromSource(std::string sourceString, std::string sourceType);

            // sets the internal bits representation from the source string (treating as hex)
            void _setInternalsFromHex(std::string sourceString);

            // sets the internal bits representation from the source string (treating as base64)
            void _setInternalsFromBase64(std::string sourceString);

            void _setInternalsFromBits(std::string sourceString);

            // convenience func used during translations
            void _setBase64CharFromBits(int bitArrayStartPos, int uintStartPos, int base64Index);

            // set the base64 string representation from internal bits
            void _setBase64StrFromBits();

            // set the bit string representation from internal bits
            void _setBitStrFromBits();

            // set the hex string representation from internal bits
            void _setHexStrFromBits();

            // get a base64 char from the given int
            char _getBase64CharFromBitVal(uint8_t bitVal);
    };   
}

#endif // UINT64_BITS_H__
