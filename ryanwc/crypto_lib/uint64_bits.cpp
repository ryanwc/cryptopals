#include <iostream>
#include <map>
#include <math.h>
#include <memory>

#include "uint64_bits.h"


namespace CustomCrypto {

    // Implementation that lazily calculates and memoizes the string representations.
    Uint64Bits::Uint64Bits(std::string sourceString, std::string sourceType) {
        if (sourceType.compare("hex") != 0) {
            throw std::invalid_argument("only known translation is for 'hex' source type");
        }
        _bits = NULL;
        _hexRepresentation = NULL;
        _base64Representation = NULL;
        _numBits = 0;
        _sourceString = sourceString;
        _sourceType = sourceType;
        _setBitsFromSource();
    }

    Uint64Bits::~Uint64Bits() {
        if (_bits) {
            free(_bits);
        }
        if (_hexRepresentation) {
            free(_hexRepresentation);
        }
        if (_base64Representation) {
            free(_base64Representation);
        }
    }

    int Uint64Bits::GetNumBits() {
        return _numBits;
    }

    std::string Uint64Bits::GetBase64Representation() {
        if (_sourceType.compare("base64") == 0) {
            return _sourceString;
        }
        if (_base64Representation) {
            return _base64Representation;
        } 

        _setBase64StrFromBits();
        return _base64Representation;
    }

    std::string Uint64Bits::GetHexRepresentation() {
        if (_sourceType.compare("hex") == 0) {
            return _sourceString;
        }
        //if (_hexRepresentation) {
        //    return _hexRepresentation;
        //}
        throw std::runtime_error("get hex representation not implemented for non-hex source");
    }

    std::shared_ptr<uint64_t*> Uint64Bits::GetBits() {
        throw std::runtime_error("not implemented");
    }


    void Uint64Bits::_setBitsFromSource() {
        if (_sourceType.compare("hex") == 0) {
            _setBitsFromHex();
        }
    }

    inline void Uint64Bits::_setBitsFromHex() {

        int bitArrPos = 0;
        int bitsLeftInPos = 64;

        int srcStrLen = _sourceString.length();

		if (srcStrLen % 2 != 0) {
			throw std::invalid_argument("purported hex string is not even length: " + _sourceString);
		}

        _numBits = srcStrLen * 4;
        int numSetBitsInLast64 = _numBits % 64;
        int uint64ArrayLen = int(ceil(_numBits / 64.0)) + 1;

        if (_bits) {
            free(_bits);
        }
        if (_base64Representation) {
            free(_base64Representation);
            _base64Representation = NULL;
        }
        if (_hexRepresentation) {
            free(_hexRepresentation);
            _hexRepresentation = NULL;
        }

        _bits = (uint64_t*) malloc(sizeof(uint64_t) * (uint64ArrayLen+1));  // plus one for base64 convience, see below

        _bits[0] = 0b0;

        int sourceStrPos = 0;
        while (sourceStrPos < srcStrLen) {
            _bits[bitArrPos] <<= 4;
            switch (_sourceString[sourceStrPos]) {
                case '0':
                    _bits[bitArrPos] |= 0b0000;
                    break;
                case '1':
                    _bits[bitArrPos] |= 0b0001;
                    break;
                case '2':
                    _bits[bitArrPos] |= 0b0010;
                    break;
                case '3':
                    _bits[bitArrPos] |= 0b0011;
                    break;
                case '4':
                    _bits[bitArrPos] |= 0b0100;
                    break;
                case '5':
                    _bits[bitArrPos] |= 0b0101;
                    break;
                case '6':
                    _bits[bitArrPos] |= 0b0110;
                    break;
                case '7':
                    _bits[bitArrPos] |= 0b0111;
                    break;
                case '8':
                    _bits[bitArrPos] |= 0b1000;
                    break;
                case '9':
                    _bits[bitArrPos] |= 0b1001;
                    break;
                case 'A':
                case 'a':
                    _bits[bitArrPos] |= 0b1010;
                    break;
                case 'B':
                case 'b':
                    _bits[bitArrPos] |= 0b1011;
                    break;
                case 'C':
                case 'c':
                    _bits[bitArrPos] |= 0b1100;
                    break;
                case 'D':
                case 'd':
                    _bits[bitArrPos] |= 0b1101;
                    break;
                case 'E':
                case 'e':
                    _bits[bitArrPos] |= 0b1110;
                    break;
                case 'F':
                case 'f':
                    _bits[bitArrPos] |= 0b1111;
                    break;
                default:
                    std::string errMessage = "given hex string has invalid hexadecimal char: ";
                    errMessage.push_back(_sourceString[sourceStrPos]);
                    throw std::invalid_argument(errMessage);
            }

            sourceStrPos += 1;
            bitsLeftInPos -= 4;
            if (bitsLeftInPos < 1) {
                bitArrPos += 1;
                _bits[bitArrPos] = 0b0;
                bitsLeftInPos = 64;
            } 
        }

        // put trailing 0s for convenience when converting to base64
        _bits[uint64ArrayLen-1] = 0b0;
        // ensure bits we care about in last uint64 are most significant
        _bits[uint64ArrayLen-2] <<= 64 - numSetBitsInLast64;
    }

    // Set the base64CharArray[base64Index] according to bitArray[bitArrayStartPos:bitArrayStartPos+6]
    // e.g. if bitArray = {0, 0, 1, 0, 1, 1, 0, 1} and bitArrayStartPos = 0, then base64CharArray[base64Index] set to 'T' (bits "001011")
    // Caller's responsibility to pass correct array/uint start positions
    inline void Uint64Bits::_setBase64CharFromBits(
        uint64_t bitArray[], int bitArrayStartPos, int uintStartPos, char base64CharArray[], int base64Index) {

        // isolate the next bits we care about
        uint64_t firstBitPortion = bitArray[bitArrayStartPos];
        uint8_t bitVal;
        uint8_t intermediate; // doing shifting to clear top bits in one line just preserves original val
        if (uintStartPos > 58) {
            int bitsInFirstPortion = 64 - uintStartPos;
            bitVal = firstBitPortion;
            // slide first portion over left while zeroing most sig bits
            intermediate = bitVal << (8 - bitsInFirstPortion);
            bitVal = intermediate >> 2;  // start first portion at first bits that count for the int val
            int bitsInSecondPortion = 6 - bitsInFirstPortion;
            uint64_t secondBitPortion = bitArray[bitArrayStartPos+1];
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

        base64CharArray[base64Index] = ASCII_groupStartVal + groupOffset;
    }

    void Uint64Bits::_setBase64StrFromBits() {

        int numBase64Chars = _numBits / 6;
        int base64StrLen = numBase64Chars;
        if (_numBits % 6 != 0) {
            base64StrLen += 1;
            while (base64StrLen % 4 != 0) {
                base64StrLen += 1;
            }
        }

        _base64Representation = (char*) malloc(sizeof(char) * (base64StrLen+1));  // +1 for null char string terminator
        int numBitsConverted;
        int uint64ArrIndex = 0;
        int base64CharIndex = 0;
        int uintBitStartPos = 0;
        for (numBitsConverted = 0; numBitsConverted < _numBits; numBitsConverted += 6) {
            _setBase64CharFromBits(_bits, uint64ArrIndex, uintBitStartPos, _base64Representation, base64CharIndex);
            base64CharIndex += 1;
            uintBitStartPos += 6;
            if (uintBitStartPos >= 64) {
                // could also do mod but is mod arithmetic slower?
                uintBitStartPos -= 64;
                uint64ArrIndex += 1;
            }
        }

        while (base64CharIndex < base64StrLen) {
            _base64Representation[base64CharIndex] = '=';  // base64 padding char padding
            base64CharIndex += 1;
        }

        _base64Representation[base64CharIndex] = '\0';
    }

    void Uint64Bits::_setHexStrFromBits() {
        throw std::runtime_error("not implemented");
    }

    void Uint64Bits::_setBitsFromBase64() {
        throw std::runtime_error("not implemented");
    }
}
