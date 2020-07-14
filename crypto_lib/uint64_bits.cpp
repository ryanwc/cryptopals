#include <iostream>
#include <map>
#include <math.h>
#include <memory>

#include "uint64_bits.h"


namespace CustomCrypto {

    // Implementation that:
    // - uses naked pointers for internal bits and string representations.
    // - lazily calculates and memoizes the string representations.
    Uint64Bits::Uint64Bits(std::string sourceString, std::string sourceType, bool preserveLeadingZeroes) {
        if (sourceString.length() < 1) {
            throw std::invalid_argument("sourceString cannot be empty");
        }
        _bits = NULL;
        _hexRepresentation = NULL;
        _base64Representation = NULL;
        _bitRepresentation = NULL;
        _numBits = 0;
        _numUint64s = 0;
        _numPaddingBits = 0;
        _preserveLeadingZeroes = preserveLeadingZeroes;
        _initInternalsFromSource(sourceString, sourceType);
    }

    Uint64Bits::Uint64Bits(std::unique_ptr<uint64_t[]> bits, int numBits, int numUint64s, int numPaddingBits, bool preserveLeadingZeroes) {
        _bits = bits.release();
        _hexRepresentation = NULL;
        _base64Representation = NULL;
        _bitRepresentation = NULL;
        _numBits = numBits;
        _numUint64s = numUint64s;
        _numPaddingBits = numPaddingBits;
        _preserveLeadingZeroes = preserveLeadingZeroes;
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

    bool Uint64Bits::GetAnyLeadingZeroesWerePreserved() const {
        return _preserveLeadingZeroes;
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

    std::unique_ptr<Uint64Bits> Uint64Bits::XOR(const Uint64Bits & otherBits, bool preserveLeadingZeroes) {
        
        int numOtherBits = otherBits.GetNumBits();
        int numOtherUint64s = otherBits.GetNumUint64s();
        int numOtherPadding = otherBits.GetNumPaddingBits();
        uint64_t* otherBitsInternal = otherBits.GetBits().release();  // want same type as this _bits

        // iterate and do xors
        bool myBitsLonger = _numBits >= numOtherBits;

        uint64_t* longerArray;
        uint64_t* shorterArray;
        int longerArrSize;
        int numLongerPadding;
        int numLongerBits;

        if (myBitsLonger) {
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

        free(otherBitsInternal);

        // TODO: preserve bits or not.

        int numSetBits = numLongerBits;
        int newArrSize = longerArrSize;
        int newPadding = numLongerPadding;
        if (!_preserveLeadingZeroes) {
            // figure out what the first set bit is, and fix numSet/padding/array size as necessary.
            // probably several ways to do this. e.g. 63 cases, or "test against 0, 1, and *2 until less than". 
            // we'll do bit shifting right.
            int testBitPos = numLongerPadding;
            int testArrPos = 0;
            while (numSetBits > 0) {
                if (testBitPos > 63) {
                    testBitPos = 0;
                    newPadding = 0;
                    newArrSize -= 1;
                    testArrPos += 1;
                }
                // test if that bit is set
                if (xorBits[testArrPos] >> (63 - testBitPos) != 0) {
                    break;
                }
                numSetBits -= 1;
                newPadding += 1;
            }

            // in edge case of all zero (i.e., all bits xor'd to 0), everything should be OK
            // arr size should already be 1, and numSetBits is 0 and newPadding is 64

            int sizeDiff = longerArrSize - newArrSize;
            if (sizeDiff > 0) {
                // there were enough sig bits the same between the two that now we can use less uint64s
                auto shorterNewBits = std::make_unique<uint64_t[]>(newArrSize);
                for (int i = 0; i < newArrSize; i ++) {
                    shorterNewBits[i] = xorBits[i + sizeDiff];
                }
                return std::make_unique<Uint64Bits>(std::move(shorterNewBits), numSetBits, newArrSize, newPadding);
            }
        }
        return std::make_unique<Uint64Bits>(std::move(xorBits), numSetBits, newArrSize, newPadding);
    }

    void Uint64Bits::_initInternalsFromSource(std::string sourceString, std::string sourceType) {
        if (sourceType.compare("hex") == 0) {
            _setInternalsFromHex(sourceString);
        }
        else {
		    throw std::invalid_argument("source types supported: ['hex']");
        }
    }

    inline void Uint64Bits::_setInternalsFromHex(std::string hexString) {

        int srcStrLen = hexString.length();

        int numLeadingZeroes = 0;
        int zeroesTestPos = 0;

        // does not count final zero in e.g. "0000" as a leading zero
        if (!_preserveLeadingZeroes) {
            while (hexString[zeroesTestPos] == '0' && zeroesTestPos < srcStrLen - 1) {
                numLeadingZeroes += 1;
                zeroesTestPos += 1;
            }
        }

        int numNonLeadingZeroBits = (srcStrLen - numLeadingZeroes) * 4;
        _numUint64s = int(ceil(numNonLeadingZeroBits / 64.0));
        _bits = (uint64_t*) malloc(sizeof(uint64_t) * _numUint64s);

        int sourceStrPos = srcStrLen - 1;
        int bitArrPos = _numUint64s;
        int bitsAssignedThis64 = 64;
        int nextPadding;
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
                    nextPadding = 4;
                    break;
                case '1':
                    nextVal = 0b0001;
                    nextPadding = 3;
                    break;
                case '2':
                    nextVal = 0b0010;
                    nextPadding = 2;
                    break;
                case '3':
                    nextVal = 0b0011;
                    nextPadding = 2;
                    break;
                case '4':
                    nextVal = 0b0100;
                    nextPadding = 1;
                    break;
                case '5':
                    nextVal = 0b0101;
                    nextPadding = 1;
                    break;
                case '6':
                    nextVal = 0b0110;
                    nextPadding = 1;
                    break;
                case '7':
                    nextVal = 0b0111;
                    nextPadding = 1;
                    break;
                case '8':
                    nextVal = 0b1000;
                    nextPadding = 0;
                    break;
                case '9':
                    nextVal = 0b1001;
                    nextPadding = 0;
                    break;
                case 'A':
                case 'a':
                    nextVal = 0b1010;
                    nextPadding = 0;
                    break;
                case 'B':
                case 'b':
                    nextVal = 0b1011;
                    nextPadding = 0;
                    break;
                case 'C':
                case 'c':
                    nextVal = 0b1100;
                    nextPadding = 0;
                    break;
                case 'D':
                case 'd':
                    nextVal = 0b1101;
                    nextPadding = 0;
                    break;
                case 'E':
                case 'e':
                    nextVal = 0b1110;
                    nextPadding = 0;
                    break;
                case 'F':
                case 'f':
                    nextVal = 0b1111;
                    nextPadding = 0;
                    break;
                default:
                    std::string errMessage = "given hex string has invalid hexadecimal char: ";
                    errMessage.push_back(hexString[sourceStrPos]);
                    throw std::invalid_argument(errMessage);
            }
            _bits[bitArrPos] |= nextVal << bitsAssignedThis64;
            sourceStrPos -= 1;
            bitsAssignedThis64 += 4;
            _numBits += 4;
        } 
        while (sourceStrPos - numLeadingZeroes >= 0);

        _numBits -= _preserveLeadingZeroes ? 0 : nextPadding;
        _numPaddingBits = 64 - bitsAssignedThis64 + (_preserveLeadingZeroes ? 0 : nextPadding);
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

        /*
        https://docs.oracle.com/cd/E18150_01/javadocs/DevelopmentKit/com/stc/connector/framework/util/Base64.html
        The Base64 Content-Transfer-Encoding is designed to represent arbitrary sequences of octets in a form 
        that need not be humanly readable. The encoding and decoding algorithms are simple, but the encoded data 
        are consistently only about 33 percent larger than the unencoded data. This encoding is virtually identical 
        to the one used in Privacy Enhanced Mail (PEM) applications, as defined in RFC 1421.

        So, it makes no sense to say for example "translate 0b1 to base64".
        A better command is "translate 0b00000001 to base64".
        Since we're allowing non-even hex strings, that also means we could have a non-multiple-of-8 _numBits.
        So let's take care of that.
        */
        int octetOverflow = _numBits % 8;
        int octetNumBits = octetOverflow == 0 ? _numBits : _numBits + (8 - octetOverflow);
        // handle edge case of 0 bits (i.e., these bits represent "no data" and leading zeroes not preserved)
        if (_numBits == 0) {
            octetNumBits = 8;
        }
        int base64StrLen = octetNumBits / 6;
        int numPaddingZeroes = 0;
        int overflow = octetNumBits % 6;
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
        int uintBitStartPos = _numPaddingBits - (octetNumBits - _numBits);
        int numBitsConverted = 0;
        int bitsToConvert = numPaddingZeroes > 0 ? octetNumBits - (6 - numPaddingZeroes) : octetNumBits;
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

        int hexStrLen = _numBits < 4 ? 1 : int(ceil(_numBits / 4.0));
        int overflow = _numBits % 4;
        int numFrontZeroes = overflow == 0 ? 0 : 4 - overflow;
        // handle edge case
        if (_numBits == 0) {
            numFrontZeroes = 4;
        }


        _hexRepresentation = (char*) malloc(sizeof(char) * (hexStrLen + 1)); // for null terinator
        _hexRepresentation[hexStrLen] = '\0';

        int bitPos = _numPaddingBits - numFrontZeroes;
        int bitArrPos = 0;
        int hexStrPos = 0;
        uint8_t fourBits;
        uint8_t intermediate;
        uint64_t currBitsChunk = _bits[0];
        while (hexStrPos < hexStrLen) {
            // isolate the 4 bits we care about in the 4 least sig bits of these 8
            if (bitPos <= 56) {
                // bitshift undefined for negative operands
                fourBits = currBitsChunk >> (56 - bitPos);
                fourBits >>= 4;  // if this doesnt work need the 'intermediate' trick
            }
            else {
                // we know this is the least 4 bits in the 64
                fourBits = currBitsChunk;
                intermediate = fourBits << 4;
                fourBits = intermediate >> 4;
            }

            switch (fourBits) {
                case 0b0000:
                    _hexRepresentation[hexStrPos] = '0';
                    break;
                case 0b0001:
                    _hexRepresentation[hexStrPos] = '1';
                    break;
                case 0b0010:
                    _hexRepresentation[hexStrPos] = '2';
                    break;
                case 0b0011:
                    _hexRepresentation[hexStrPos] = '3';
                    break;
                case 0b0100:
                    _hexRepresentation[hexStrPos] = '4';
                    break;
                case 0b0101:
                    _hexRepresentation[hexStrPos] = '5';
                    break;
                case 0b0110:
                    _hexRepresentation[hexStrPos] = '6';
                    break;
                case 0b0111:
                    _hexRepresentation[hexStrPos] = '7';
                    break;
                case 0b1000:
                    _hexRepresentation[hexStrPos] = '8';
                    break;
                case 0b1001:
                    _hexRepresentation[hexStrPos] = '9';
                    break;
                case 0b1010:
                    _hexRepresentation[hexStrPos] = 'A';
                    break;
                case 0b1011:
                    _hexRepresentation[hexStrPos] = 'B';
                    break;
                case 0b1100:
                    _hexRepresentation[hexStrPos] = 'C';
                    break;
                case 0b1101:
                    _hexRepresentation[hexStrPos] = 'D';
                    break;
                case 0b1110:
                    _hexRepresentation[hexStrPos] = 'E';
                    break;
                case 0b1111:
                    _hexRepresentation[hexStrPos] = 'F';
                    break;
                default:
                    throw std::invalid_argument("hex translation missed a 4-bit permuation");
            }
            bitPos += 4;
            hexStrPos += 1;
            if (bitPos > 63) {
                bitPos = 0;
                bitArrPos += 1;
                currBitsChunk = _bits[bitArrPos];
            }
        }
    }

    void Uint64Bits::_setBitStrFromBits() {

        _bitRepresentation = (char*) malloc(sizeof(char) * (_numBits + 1));
        _bitRepresentation[_numBits] = '\0';

        int currTestPos = _numPaddingBits;
        int currArrPos = 0;
        int bitStrPos = 0;
        
        // handle special case
        if (_numBits == 0) {
            _bitRepresentation[1] = '\0';
            _bitRepresentation[0] = '0';
            return;
        }

        while (bitStrPos < _numBits) {
            // test if the bit is set
            _bitRepresentation[bitStrPos] = (_bits[currArrPos] >> (63 - currTestPos)) & 1 ? '1' : '0';
            bitStrPos += 1;
            currTestPos += 1;
            if (currTestPos > 63) {
                currTestPos = 0;
                currArrPos += 1;
            }
        }
    }

    void Uint64Bits::_setInternalsFromBase64(std::string sourceString) {
        throw std::runtime_error("not implemented");
    }

    void Uint64Bits::_setInternalsFromBits(std::string sourceString) {
        throw std::runtime_error("not implemented");
    }
}
