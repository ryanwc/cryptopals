# classix-crypto

Cryptography challenges for fun and... well, just fun.

https://cryptopals.com/

All challenges implemented in C++.

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
