# classix-crypto

Cryptography challenges for fun and... well, just fun.

https://cryptopals.com/

All challenges implemented in C++.

## Testing

The tests in this project use [googletest](https://github.com/google/googletest). There are multiple ways to include googletest in a project, but for
simplicity I've installed it as a system library and linked against that for each test executable. See [here](https://github.com/google/googletest) for instructions on how to do that on Linux.

## Build Info

- requires cmake and C++ 11 compiler

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
