# classix-crypto

Cryptography challenges for fun and... well, just fun.

https://cryptopals.com/

All challenges implemented in C++.

## Set 1: Basics

### Why Base64 is Useful

Great, we can encode 3 bytes with 4 text characters? How is that useful when we can just encode 3 bytes with... 3 text characters?
Seems bad, we're increasing the payload size.

https://stackoverflow.com/questions/3538021/why-do-we-use-base64

TLDR: Some ASCII characters may get corrupted when transferred across a network. So, use encoding to "known safe" ASCII chars.
