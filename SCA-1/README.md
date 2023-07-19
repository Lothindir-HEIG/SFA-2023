This is a white-box implementation of AES-128 encryption.
The developer decided to print out information about the beginning of the 10th round state, namely, every state byte.
The program will output `[16-hex-bytes][space][16-hex-bytes]`:

- the first [16-hex-bytes] is the ciphertext
- the second [16-hex-bytes] is the "10th round state bytes."

`nc iict-mv330-sfa 4000`