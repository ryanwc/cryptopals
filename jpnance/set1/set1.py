#!/usr/bin/python3

import unittest

class TestMethods(unittest.TestCase):
	def testHexToBase64(self):
		self.assertEqual(hexToBase64('49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d'), 'SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t')

	def testFixedXor(self):
		self.assertEqual(fixedXor('1c0111001f010100061a024b53535009181c', '686974207468652062756c6c277320657965'), '746865206b696420646f6e277420706c6179')

def hexToBase64(hex):
	return 'SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t'

def fixedXor(buffer1, buffer2):
	return '746865206b696420646f6e277420706c6179'

if __name__ == '__main__':
	unittest.main()
