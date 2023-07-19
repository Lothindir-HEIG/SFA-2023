This is a white-box implementation of AES-128 encryption.
An attacker got some information from memory, namely, he got an array of 17 elements.
He knows that the first element is a Hamming weight of the mask HW(mask).
The other 16 values is the masked state at the beginning of the 10th round: (S10[i] xor mask, i from 0 to 15).The binary will output `[16-hex-bytes][space][100-hex-bytes]`:

- the first [16-hex-bytes] is the ciphertext
- the second [17-hex-bytes] is the "Hamming weight of the mask and 16 masked bytes of the round state at the beginning of the 10th round.

`nc iict-mv330-sfa.einet.ad.eivd.ch. 4004` 