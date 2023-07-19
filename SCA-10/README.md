In this task you will work with software AES-128 running on ESP32.

This AES-128 is implemented with T-tables; however, designers put a random delay before each "atom" operation, such as one MixColumn computation (see code_sample.txt in support folder).

You have access to plaintexts, ciphertexts and associated traces. Oscilloscope snapshot shows an entire AES execution.

The measurements were performed with LeCroy WaveRunner Zi625 and a differential probe