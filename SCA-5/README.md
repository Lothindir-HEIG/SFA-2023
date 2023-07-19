In this task you will analyse software T-table AES-128 implementation running on ESP32 (see the code_sample.txt in the support folder).

The software implementation does not contain any protection against side-channel attacks.

During one AES-128 execution a plaintext, a ciphertext and a real power trace of 5,000 samples were recorded.

10,000 encryptions were performed. Associated data (a plaintext, a ciphertext and a trace) was stored in HDF5 file.

One oscilloscope snapshot shows an entire AES execution, however, only the part related to encryption process, i.e., a part shown from 50 us to 60 us on the oscilloscope snapshot, is available for the analysis.

The measurements were performed with LeCroy WaveRunner Zi625 and a differential probe.
