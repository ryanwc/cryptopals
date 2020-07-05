#ifndef UINT64_BITS_H__
#define UINT64_BITS_H__

#include <iostream>
#include <memory>


namespace CustomCrypto {

    // Represent a source string as bits in a series of uint64_t.
    // Assumes total number of bits represented by given source string fits in int data type.
    // (Dev note: could refactor for factory pattern and/or subclassing, but seems OK for now.)
    class Uint64Bits {

        public:

            Uint64Bits(std::string sourceString, std::string sourceType);
            ~Uint64Bits();

			// get the total number of bits this Uint64Bits represents
            int GetNumBits();

			// Get a base64 string representation of the bits
            std::string GetBase64Representation();

			// Get a hex string representation of the bits
            std::string GetHexRepresentation();

            // Get a bit string representation of the bits
			std::string GetBitRepresentation();

            // Get a pointer to an array of uint64_t which is a copy of the internal
            // representation of the bits.
            std::shared_ptr<uint64_t*> GetBits();

        private:

            // the source string that this was constructed with
            std::string _sourceString;
            // the type of the source string (e.g. 'hex')
            std::string _sourceType;
            // the hex representation of these bits
            char* _hexRepresentation;
            // the base64 representaton of these bits
            char* _base64Representation;
            // the number of bits this represents
            int _numBits;
            // the internal representation of the bits
            uint64_t* _bits;

            // sets the internal bits representation from the source string/type
            void _setBitsFromSource();

            // sets the internal bits representation from the source string (treating as hex)
            void _setBitsFromHex();

            // sets the internal bits representation from the source string (treating as base64)
            void _setBitsFromBase64();

            // convenience func used during translations
            void _setBase64CharFromBits(
                uint64_t bitArray[], int bitArrayStartPos, int uintStartPos, char base64CharArray[], int base64Index);

            // set the base64 string representation from internal bits
            void _setBase64StrFromBits();

            // set the hex string representation from internal bits
            void _setHexStrFromBits();
    };   
}

#endif // UINT64_BITS_H__
