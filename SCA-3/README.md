This is a white-box implementation of AES-128 encryption.
An attacker got some information from memory, namely, he got an array of 100 elements.
He knows that among 100 elements, 16 of them describe the 10th round state, namely, Hamming weight of every state byte.
The other 84 values seems to be just random. However, an attacker does not know where the correct 16 bytes are located.
The program will output `[16-hex-bytes][space][100-hex-bytes]`:

- the first [16-hex-bytes] is the ciphertext
- the second [100-hex-bytes] is the "Hamming weight of the 10th round input state bytes" plus random values

`nc iict-mv330-sfa 4002`