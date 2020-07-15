# classix-crypto

Cryptography challenges for fun and... well, just fun.

https://cryptopals.com/

All challenges implemented in C++.

## Web App

The web_app dir has a Python Flask app. Right now it doesn't do much and isnt deployed. Eventually I will set up CICD that will compile/test/deploy the app on e.g. AWS so that there is a UI for people to use the completed crypto challenges.

The idea is that Python is a pleasant language in which to develop web applications, but maybe not as fast as C++ for doing intense cryptography work. I already saw evidence of this, since my C++ implementation of challenge 1 is over 10x faster than a Python implementation I saw (but maybe that Python code isn't optimized at all).

Regardless of the merit in this particular app, it is an interesting archicture that comes up in the real world (e.g., using C++ executable to do graphics rendering and writing a Python API to make the results available). So, I thought it would be fun to explore the possibilities here.

## Testing

The tests in this project use [googletest](https://github.com/google/googletest). There are multiple ways to include googletest in a project, but for
simplicity I've installed it as a system library and linked against that for each test executable. See [here](https://github.com/google/googletest) for instructions on how to do that on Linux.

## Build Info

- uses CMake with top level project at root of dir and each individual challenge added as a subdirectory
- requires cmake version 3.10+ and C++ 11 compiler

Example building set 1 exercise 1:

From dir `./set1/hex_to_base64`:

- `cmake [desired flags]`
- `make`

Now the program and test program binaries are in `./set1/hex_to_base64/bin`.

## Set 1: Basics

### Why Base64 is Useful

Great, we can encode 3 bytes with 4 text characters? How is that useful when we can just encode 3 bytes with... 3 text characters?
Seems bad, we're increasing the payload size.

https://stackoverflow.com/questions/3538021/why-do-we-use-base64

TLDR: Some ASCII characters may get corrupted when transferred across a network. So, use encoding to "known safe" ASCII chars.

### Why Padding is Useful in Base64

https://www.pixelstech.net/article/1457585550-How-does-Base64-work

TLDR: Without padding, concatenating two Base64 encoded strings then decoding will give the wrong result.

#### Challenge 1: Hex to Base 64

- [Challenge 1 description](https://cryptopals.com/sets/1/challenges/1)
- Originally implementation translated hex string to array of bools representing the binary hex string val, then translated to base 64 from that
  - This implementation was about 3x faster than another (working but unknown how much thought put into optimization) python implementation
- New implementation translates hex string to array of uint64_t where the concatenation of binary vals of each uint64_t in the array represents the binary hex string val, then translates to base 64 from that
  - New implementation was 4x faster than old implementation, so over an order of magnitude faster than the (again, maybe not optimized) python implementation
  - Interestingly, this timing changed again when I switched to C++17 standard and using some unique pointers in certain places instead of naked pointers. Both of these caused a slowdown, cumulatively about half as fast as before. I settled on using unique pointers only for the external API to a class I created for challenge 2, which seemed like a good compromise.

### Challenge 2: XOR Hex Strings

- [Challenge 2 description](https://cryptopals.com/sets/1/challenges/2)
- I created a class called Uint64Bits that represnts some bits as an array uint64_t and provides several related functions (converting to hex, base64, bitstring, etc) that I hope can be reused in the other challenges.

### Challenge 3: Single-Byte XOR Cipher

- [Challenge 3 description](https://cryptopals.com/sets/1/challenges/3)
- [Here's an explanation of what a single-byte XOR cipher is](https://wiki.bi0s.in/crypto/xor/#single-byte-xor-cipher)
