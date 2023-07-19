In this task you will work with hardware AES-128 running on ESP32. This engine is started by protocol:

mbedtls_aes_init( &aes ); mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, 128 ); mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)input, output); mbedtls_aes_free( &aes );

You have access to plaintexts, ciphertexts and associated traces. Oscilloscope snapshots (see below) show an entire AES execution, i.e., mbedtls_aes_crypt_ecb process.

You will have only the part related to encryption process, i.e., a part shown from 8 us to 8.5 us on the second oscilloscope snapshot.

The measurements were performed with LeCroy WaveRunner Zi625 and a differential probe
