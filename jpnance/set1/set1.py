#!/usr/bin/python3

import unittest

class TestMethods(unittest.TestCase):
	def testHexToBase64(self):
		self.assertEqual(hexToBase64('49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d'), 'SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t')

def hexToBase64(hex):
	conversionString = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
	outputString = ''

	for i in range(int(len(hex) / 3)):
		offset = i * 3

		outputString += conversionString[(int(hex[offset + 0], 16) << 2) | (int(hex[offset + 1], 16) >> 2)]
		outputString += conversionString[((int(hex[offset + 1], 16) & 0x3) << 4) | (int(hex[offset + 2], 16))]


	return outputString

if __name__ == '__main__':
	unittest.main()
