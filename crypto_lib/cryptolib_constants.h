#ifndef CRYPTOLIB_CONSTANTS_H__
#define CRYPTOLIB_CONSTANTS_H__

#include <unordered_map>
#include <unordered_set>


namespace CryptolibConstants {

    // char likelihoods taken from:
    // https://en.wikipedia.org/wiki/Letter_frequency
    // treats capital letters and lower case letters as same
    // only handles English alphabet chars
	const std::unordered_map<char, long double> charLogLikelihood = {
		{'A', logl(0.08497)},
		{'a', logl(0.08497)},
		{'B', logl(0.01492)},
		{'b', logl(0.01492)},
		{'C', logl(0.02202)},
		{'c', logl(0.02202)},
        {'D', logl(0.04253)},
        {'d', logl(0.04253)},
        {'E', logl(0.11162)},
        {'e', logl(0.11162)},
        {'F', logl(0.02228)},
        {'f', logl(0.02228)},
        {'G', logl(0.02015)},
        {'g', logl(0.02015)},
        {'H', logl(0.06094)},
        {'h', logl(0.06094)},
        {'I', logl(0.07546)},
        {'i', logl(0.07546)},
        {'J', logl(0.00153)},
        {'j', logl(0.00153)},
        {'K', logl(0.01292)},
        {'k', logl(0.01292)},
        {'L', logl(0.04025)},
        {'l', logl(0.04025)},
        {'M', logl(0.02406)},
        {'m', logl(0.02406)},
        {'N', logl(0.06749)},
        {'n', logl(0.06749)},
        {'O', logl(0.07507)},
        {'o', logl(0.07507)},
        {'P', logl(0.01929)},
        {'p', logl(0.01929)},
        {'Q', logl(0.00095)},
        {'q', logl(0.00095)},
        {'R', logl(0.07587)},
        {'r', logl(0.07587)},
        {'S', logl(0.06327)},
        {'s', logl(0.06327)},
        {'T', logl(0.09356)},
        {'t', logl(0.09356)},
        {'U', logl(0.02758)},
        {'u', logl(0.02758)},
        {'V', logl(0.00978)},
        {'v', logl(0.00978)},
        {'W', logl(0.02560)},
        {'w', logl(0.02560)},
        {'X', logl(0.00150)},
        {'x', logl(0.00150)},
        {'Y', logl(0.01994)},
        {'y', logl(0.01994)},
        {'Z', logl(0.00077)},
        {'z', logl(0.00077)}
	};

    std::unordered_set<char> skipChar = {
        ' ',
        '.',
        ',',
        '!',
        '?',
        '-',
        '\''
    };
}

#endif // CRYPTOLIB_CONSTANTS_H
