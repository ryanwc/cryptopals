#include <iostream>
#include <map>
#include <math.h>
#include <memory>

#include "uint64_bits.h"


namespace CustomCrypto {

    // Implementation that:
    // - uses naked pointers for internal bits and string representations.
    // - lazily calculates and memoizes the string representations.
    Uint64Bits::Uint64Bits(std::string sourceString, std::string sourceType) {
        _bits = NULL;
        _hexRepresentation = NULL;
        _base64Representation = NULL;
        _bitRepresentation = NULL;
        _numBits = 0;
        _numUint64s = 0;
        _numPaddingBits = 0;
        _initInternalsFromSource(sourceString, sourceType);
    }

    Uint64Bits::Uint64Bits(std::unique_ptr<uint64_t[]> bits, int numBits, int numUint64s, int numPaddingBits) {
        _bits = bits.release();
        _hexRepresentation = NULL;
        _base64Representation = NULL;
        _bitRepresentation = NULL;
        _numBits = numBits;
        _numUint64s = numUint64s;
        _numPaddingBits = numPaddingBits;
    }

    Uint64Bits::~Uint64Bits() {
        if (_bits) {
            free(_bits);
        }
        if (_base64Representation) {
            free(_base64Representation);
        }
        if (_hexRepresentation) {
            free(_hexRepresentation);
        }
        if (_bitRepresentation) {
            free(_bitRepresentation);
        }
    }

    int Uint64Bits::GetNumBits() const {
        return _numBits;
    }

    int Uint64Bits::GetNumUint64s() const {
        return _numUint64s;
    }

    int Uint64Bits::GetNumPaddingBits() const {
        return _numPaddingBits;
    }

    std::string Uint64Bits::GetBase64Representation() {
        if (_base64Representation) {
            return _base64Representation;
        } 

        _setBase64StrFromBits();
        return _base64Representation;
    }

    std::string Uint64Bits::GetHexRepresentation() {
        if (_hexRepresentation) {
            return _hexRepresentation;
        }

        _setHexStrFromBits();
        return _hexRepresentation;
    }

    std::string Uint64Bits::GetBitRepresentation() {
        if (_bitRepresentation) {
            return _bitRepresentation;
        }

        _setBitStrFromBits();
        return _bitRepresentation;
    }

    std::unique_ptr<uint64_t[]> Uint64Bits::GetBits() const {
        
        auto bitsCopy = std::make_unique<uint64_t[]>(_numUint64s);
        for (int i = 0; i < _numUint64s; i++) {
            bitsCopy[i] = _bits[i];
        }
        return bitsCopy;
    }

    std::unique_ptr<Uint64Bits> Uint64Bits::XOR(const Uint64Bits & otherBits) {
        
        int numOtherBits = otherBits.GetNumBits();
        int numOtherUint64s = otherBits.GetNumUint64s();
        int numOtherPadding = otherBits.GetNumPaddingBits();
        uint64_t* otherBitsInternal = otherBits.GetBits().release();

        // iterate and do xors
        bool myArrayLonger = _numUint64s >= numOtherUint64s;

        uint64_t* longerArray;
        uint64_t* shorterArray;
        int longerArrSize;
        int numLongerPadding;
        int numLongerBits;

        if (myArrayLonger) {
            longerArray = _bits;
            shorterArray = otherBitsInternal;
            longerArrSize = _numUint64s;
            numLongerPadding = _numPaddingBits;
            numLongerBits = _numBits;
        }
        else {
            longerArray = otherBitsInternal;
            shorterArray = _bits;
            longerArrSize = numOtherUint64s;
            numLongerPadding = numOtherPadding;
            numLongerBits = numOtherBits;
        }

        auto xorBits = std::make_unique<uint64_t[]>(longerArrSize);

        int arraySizeDiff = abs(_numUint64s - numOtherUint64s);
        int longerArrIndex;
        for (longerArrIndex = 0; longerArrIndex < arraySizeDiff; longerArrIndex++) {
            xorBits[longerArrIndex] = longerArray[longerArrIndex];
        }

        for ( ; longerArrIndex < longerArrSize; longerArrIndex++) {
            xorBits[longerArrIndex] = longerArray[longerArrIndex] ^ shorterArray[longerArrIndex - arraySizeDiff];
        }

        // done with this
        free(otherBitsInternal);

        // make and return new obj
        return std::make_unique<Uint64Bits>(std::move(xorBits), numLongerBits, longerArrSize, numLongerPadding);
    }

    void Uint64Bits::_initInternalsFromSource(std::string sourceString, std::string sourceType) {
        if (sourceType.compare("hex") == 0) {
            _setInternalsFromHex(sourceString);
        }
		throw std::invalid_argument("only source type supported: 'hex'");
    }

    inline void Uint64Bits::_setInternalsFromHex(std::string hexString) {
        // breaks if src string is len 0
        int srcStrLen = hexString.length();

		if (srcStrLen % 2 != 0) {
			throw std::invalid_argument("purported hex string is not even length: " + hexString);
		}

        _numBits = srcStrLen * 4;
        _numUint64s = int(ceil(_numBits / 64.0));
        _bits = (uint64_t*) malloc(sizeof(uint64_t) * _numUint64s);

        int sourceStrPos = srcStrLen - 1;
        int bitArrPos = _numUint64s;
        int bitsAssignedThis64 = 64;
        uint64_t nextVal;
        do {
            if (bitsAssignedThis64 >= 64) {
                bitArrPos -= 1;
                _bits[bitArrPos] = 0b0;
                bitsAssignedThis64 = 0;
            }
            switch (hexString[sourceStrPos]) {
                case '0':
                    nextVal = 0b0000;
                    break;
                case '1':
                    nextVal = 0b0001;
                    break;
                case '2':
                    nextVal = 0b0010;
                    break;
                case '3':
                    nextVal = 0b0011;
                    break;
                case '4':
                    nextVal = 0b0100;
                    break;
                case '5':
                    nextVal = 0b0101;
                    break;
                case '6':
                    nextVal = 0b0110;
                    break;
                case '7':
                    nextVal = 0b0111;
                    break;
                case '8':
                    nextVal = 0b1000;
                    break;
                case '9':
                    nextVal = 0b1001;
                    break;
                case 'A':
                case 'a':
                    nextVal = 0b1010;
                    break;
                case 'B':
                case 'b':
                    nextVal = 0b1011;
                    break;
                case 'C':
                case 'c':
                    nextVal = 0b1100;
                    break;
                case 'D':
                case 'd':
                    nextVal = 0b1101;
                    break;
                case 'E':
                case 'e':
                    nextVal = 0b1110;
                    break;
                case 'F':
                case 'f':
                    nextVal = 0b1111;
                    break;
                default:
                    std::string errMessage = "given hex string has invalid hexadecimal char: ";
                    errMessage.push_back(hexString[sourceStrPos]);
                    throw std::invalid_argument(errMessage);
            }
            _bits[bitArrPos] |= nextVal << bitsAssignedThis64;
            sourceStrPos -= 1;
            bitsAssignedThis64 += 4;
        } 
        while (sourceStrPos >= 0);

        _numPaddingBits = bitsAssignedThis64 > 0 ? (64 - bitsAssignedThis64) : 0;
    }

    // Set the base64CharArray[base64Index] according to bitArray[bitArrayStartPos:bitArrayStartPos+6]
    // e.g. if bitArray = {0, 0, 1, 0, 1, 1, 0, 1} and bitArrayStartPos = 0, then base64CharArray[base64Index] set to 'T' (bits "001011")
    // Caller's responsibility to pass correct array/uint start positions
    inline void Uint64Bits::_setBase64CharFromBits(int bitArrayStartPos, int uintStartPos, int base64Index) {

        // isolate the next bits we care about
        uint64_t firstBitPortion = _bits[bitArrayStartPos];
        uint8_t bitVal;
        uint8_t intermediate; // doing shifting to clear top bits in one line just preserves original val
        if (uintStartPos > 58) {
            int bitsInFirstPortion = 64 - uintStartPos;
            bitVal = firstBitPortion;
            // slide first portion over left while zeroing most sig bits
            intermediate = bitVal << (8 - bitsInFirstPortion);
            bitVal = intermediate >> 2;  // start first portion at first bits that count for the int val
            int bitsInSecondPortion = 6 - bitsInFirstPortion;
            uint64_t secondBitPortion = _bits[bitArrayStartPos+1];
            uint8_t secondBitVal = secondBitPortion >> (64 - bitsInSecondPortion);
            // zero top bits in second portion and slide back to OR with first portion
            intermediate = secondBitVal << (8 - bitsInSecondPortion);
            secondBitVal = intermediate >> (8 - bitsInSecondPortion);
            bitVal |= secondBitVal;
        }
        else {
            bitVal = firstBitPortion >> (64 - 6 - uintStartPos);
            // we only care about 6 bits so clear the two most significant bits
            intermediate = bitVal << 2;
            bitVal = intermediate >> 2;
        }

        _base64Representation[base64Index] = _getBase64CharFromBitVal(bitVal);
    }

    inline char Uint64Bits::_getBase64CharFromBitVal(uint8_t bitVal) {
        // map the decimal value, which represents its base64 char table index, to its contiguous ASCII group and offset within that group.
        // we could use a big ol' hashmap or switch statement instead but that seems tedious.
        // also, using a static mapping array like `base64CharArray[base64Index] = base64CharTable[int(bitDecimalVal)]` was actually a bit slower
        int ASCII_groupStartVal;
        int groupOffset;
        if (bitVal < 26) {
            ASCII_groupStartVal = 65;  // capital letters
            groupOffset = bitVal;
        }
        else if (bitVal < 52) {
            ASCII_groupStartVal = 97;  // lower case letters
            groupOffset = bitVal - 26;
        }
        else if (bitVal < 62) {
            ASCII_groupStartVal = 48;  // numbers
            groupOffset = bitVal - 52;
        }
        else if (bitVal < 63) {
            ASCII_groupStartVal = 43;  // + 
            groupOffset = 0;
        }
        else {
            ASCII_groupStartVal = 47;  // /
            groupOffset = 0;
        }

        return ASCII_groupStartVal + groupOffset;
    }

    void Uint64Bits::_setBase64StrFromBits() {

        int base64StrLen = _numBits / 6;
        int numPaddingZeroes = 0;
        int overflow = _numBits % 6;
        if (overflow != 0) {
            numPaddingZeroes = 6 - overflow;
            base64StrLen += 1;
            while (base64StrLen % 4 != 0) {
                base64StrLen += 1;
            }
        }

        _base64Representation = (char*) malloc(sizeof(char) * (base64StrLen + 1)); // for null terinator
        _base64Representation[base64StrLen] = '\0';
        int uint64ArrIndex = 0;
        int base64CharIndex = 0;
        int uintBitStartPos = _numPaddingBits;
        int numBitsConverted = 0;
        int bitsToConvert = numPaddingZeroes > 0 ? _numBits - (6 - numPaddingZeroes) : _numBits;
        while (numBitsConverted < bitsToConvert) {
            _setBase64CharFromBits(uint64ArrIndex, uintBitStartPos, base64CharIndex);
            numBitsConverted += 6;
            base64CharIndex += 1;
            uintBitStartPos += 6;
            if (uintBitStartPos >= 64) {
                // could also do mod but is mod arithmetic slower?
                uintBitStartPos -= 64;
                uint64ArrIndex += 1;
            }
        }

        if (numPaddingZeroes > 0) {
            // get last bits and pad with zeros
            uint8_t lastBits = _bits[_numUint64s-1];
            uint8_t intermediate = lastBits << (2 + numPaddingZeroes);
            lastBits = intermediate >> 2;
            _base64Representation[base64CharIndex] = _getBase64CharFromBitVal(lastBits);
            base64CharIndex += 1;
        }

        while (base64CharIndex < base64StrLen) {
            _base64Representation[base64CharIndex] = '=';  // base64 padding char padding
            base64CharIndex += 1;
        }
    }

    void Uint64Bits::_setHexStrFromBits() {
        throw std::runtime_error("not implemented");
    }

    void Uint64Bits::_setBitStrFromBits() {
        throw std::runtime_error("not implemented");
    }

    void Uint64Bits::_setInternalsFromBase64(std::string sourceString) {
        throw std::runtime_error("not implemented");
    }

    void Uint64Bits::_setInternalsFromBits(std::string sourceString) {
        throw std::runtime_error("not implemented");
    }
}
