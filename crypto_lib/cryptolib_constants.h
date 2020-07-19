#ifndef CRYPTOLIB_CONSTANTS_H__
#define CRYPTOLIB_CONSTANTS_H__

#include <unordered_map>
#include <unordered_set>


namespace CryptolibConstants {

    // char likelihoods taken from:
    // https://en.wikipedia.org/wiki/Letter_frequency
    // treats capital letters and lower case letters as same
    // only handles English alphabet chars
	const std::unordered_map<char, double> charLikelihood = {
		{'A', 0.08497},
		{'a', 0.08497},
		{'B', 0.01492},
		{'b', 0.01492},
		{'C', 0.02202},
		{'c', 0.02202},
        {'D', 0.04253},
        {'d', 0.04253},
        {'E', 0.11162},
        {'e', 0.11162},
        {'F', 0.02228},
        {'f', 0.02228},
        {'G', 0.02015},
        {'g', 0.02015},
        {'H', 0.06094},
        {'h', 0.06094},
        {'I', 0.07546},
        {'i', 0.07546},
        {'J', 0.00153},
        {'j', 0.00153},
        {'K', 0.01292},
        {'k', 0.01292},
        {'L', 0.04025},
        {'l', 0.04025},
        {'M', 0.02406},
        {'m', 0.02406},
        {'N', 0.06749},
        {'n', 0.06749},
        {'O', 0.07507},
        {'o', 0.07507},
        {'P', 0.01929},
        {'p', 0.01929},
        {'Q', 0.00095},
        {'q', 0.00095},
        {'R', 0.07587},
        {'r', 0.07587},
        {'S', 0.06327},
        {'s', 0.06327},
        {'T', 0.09356},
        {'t', 0.09356},
        {'U', 0.02758},
        {'u', 0.02758},
        {'V', 0.00978},
        {'v', 0.00978},
        {'W', 0.02560},
        {'w', 0.02560},
        {'X', 0.00150},
        {'x', 0.00150},
        {'Y', 0.01994},
        {'y', 0.01994},
        {'Z', 0.00077},
        {'z', 0.00077}
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
