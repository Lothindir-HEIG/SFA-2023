#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "aes_masked.h"
#include "esp_system.h"

/*Defferent bytes of the 32 bits word*/
#define BYTE1(word) (uint8_t)((word >> 24) & 0xff)
#define BYTE2(word) (uint8_t)((word >> 16) & 0xff)
#define BYTE3(word) (uint8_t)((word >> 8) & 0xff)
#define BYTE4(word) (uint8_t)(word & 0xff)

/*Merge 4 bytes to one word*/
#define U8_TO_U32(byte1,byte2,byte3,byte4) ((uint32_t)(byte1 << 24) | (uint32_t)(byte2 << 16) | (uint32_t)(byte3 << 8) | (uint32_t)(byte4) )

/*Multiplication by 2 for every byte in the word*/
#define XTIME_U8(x)  ((((x) & 0x7f) << 1) ^ ((((x) & 0x80) >> 7) * 0x1b))

/**********************************************************************************************
* Function which converts unsigned short int array of 16 elements into
* unsigned int array of 4 elements
* INPUT: arrIn_usint - array of 16 unsigned short int elements
* OUTPUT: arrOut_uint -  array of 4 unsigned int elements
**********************************************************************************************/
void ConvertTo_uint32(const unsigned char arr_in_uint8[16], unsigned int arr_out_uint32[4])
{
	unsigned int i = 0, j = 0;

	for (i=0;i<4;i++)
		for (j=0;j<4;j++){
			arr_out_uint32[i] <<= 8;
			arr_out_uint32[i] |= arr_in_uint8[4*i + j];
		}
}

/***********************************************************
 * Description - create a key schedule
 * Input       - key: uint32_t array[4]
 *             - w: return variable with keys
 **********************************************************/
void aes128_key_expansion_u32(const uint32_t master_key_u32[4], uint32_t w_u32[44])
{
	uint8_t i = 0;

	uint32_t rcon[10] = {	0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
												0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

  w_u32[0] = master_key_u32[0];
	w_u32[1] = master_key_u32[1];
	w_u32[2] = master_key_u32[2];
	w_u32[3] = master_key_u32[3];

	for (i=1;i<11;i++)
	{
		w_u32[i*4]   = w_u32[i*4-4] ^ U8_TO_U32(Sbox[BYTE2(w_u32[4*i-1])], Sbox[BYTE3(w_u32[4*i-1])], Sbox[BYTE4(w_u32[4*i-1])], Sbox[BYTE1(w_u32[4*i-1])]) ^ rcon[i-1];
		w_u32[i*4+1] = w_u32[i*4-3] ^ w_u32[i*4];
		w_u32[i*4+2] = w_u32[i*4-2] ^ w_u32[i*4+1];
		w_u32[i*4+3] = w_u32[i*4-1] ^ w_u32[i*4+2];
	}
}



void add_rndkey(uint8_t rndkey[16], uint8_t input[16])
{
	for (unsigned int i=0; i<16; i++)
		input[i] = input[i] ^ rndkey[i];
}

void shiftrows(uint8_t input[16])
{
	uint8_t shift_ind[16] = {0, 5, 10, 15,  4, 9, 14, 3,  8, 13, 2, 7,  12, 1, 6, 11};
	uint8_t input_cp[16];

	memcpy(input_cp, input, 16);

	for (unsigned int i=0; i<16; i++)
		input[i] = input_cp[shift_ind[i]];
}

void mixcolumn(uint8_t input[16])
{
	uint8_t output[16];

	for (unsigned int i=0; i<16; i+=4) {
		output[i+0] = XTIME_U8(input[i+0])                ^ (XTIME_U8(input[i+1]) ^ input[i+1]) ^ input[i+2]                          ^ input[i+3];
		output[i+1] = input[i+0]                          ^ XTIME_U8(input[i+1])                ^ (XTIME_U8(input[i+2]) ^ input[i+2]) ^ input[i+3];
		output[i+2] = input[i+0]                          ^ input[i+1]                          ^ XTIME_U8(input[i+2])                ^ (XTIME_U8(input[i+3]) ^ input[i+3]);
		output[i+3] = (XTIME_U8(input[i+0]) ^ input[i+0]) ^ input[i+1]                          ^ input[i+2]                          ^ XTIME_U8(input[i+3]);
	}

	memcpy(input, output,16);
}

void aes_masked_sbox(uint8_t ptext[16], uint8_t key_u8[16], uint8_t ctext[16])
{
	uint8_t mask = SECRET_CONSTANT;

	uint8_t Sbox_masked[256];

	uint32_t iByte = 0;
	uint32_t iRnd = 0;

	uint32_t key_u32[4];
	uint32_t w_u32[44];
	uint8_t state[16];

    //Create a masked Sbox
	for (iByte=0; iByte < 256; iByte++)
		Sbox_masked[iByte] = Sbox[iByte ^ mask] ^ mask;

	ConvertTo_uint32(key_u8, key_u32);

	aes128_key_expansion_u32(key_u32, w_u32);

	uint32_t *rnd_key = w_u32;

	//Add zero key
	for (iByte = 0; iByte < 16; iByte+=4){
		state[iByte+0] = ptext[iByte+0] ^ BYTE1(rnd_key[(int)(iByte / 4)]) ^ mask;
		state[iByte+1] = ptext[iByte+1] ^ BYTE2(rnd_key[(int)(iByte / 4)]) ^ mask;
		state[iByte+2] = ptext[iByte+2] ^ BYTE3(rnd_key[(int)(iByte / 4)]) ^ mask;
		state[iByte+3] = ptext[iByte+3] ^ BYTE4(rnd_key[(int)(iByte / 4)]) ^ mask;
	}

  	for (iRnd = 1; iRnd < 10; iRnd++)
  	{
  		rnd_key += 4;

  		for (iByte=0; iByte<16; iByte++)
				state[iByte] = Sbox_masked[state[iByte]];

  		shiftrows(state);

  		mixcolumn(state);

  		for (iByte = 0; iByte < 16; iByte+=4){
  			state[iByte+0] = state[iByte+0] ^ BYTE1(rnd_key[(int)(iByte / 4)]);
  			state[iByte+1] = state[iByte+1] ^ BYTE2(rnd_key[(int)(iByte / 4)]);
  			state[iByte+2] = state[iByte+2] ^ BYTE3(rnd_key[(int)(iByte / 4)]);
  			state[iByte+3] = state[iByte+3] ^ BYTE4(rnd_key[(int)(iByte / 4)]);
  		}
  	}

  	rnd_key += 4;

		for (iByte=0; iByte<16; iByte++)
			state[iByte] = Sbox_masked[state[iByte]];

  	shiftrows(state);

  	for (iByte = 0; iByte < 16; iByte+=4){
  		state[iByte+0] = state[iByte+0] ^ BYTE1(rnd_key[(int)(iByte / 4)])  ^ mask;
  		state[iByte+1] = state[iByte+1] ^ BYTE2(rnd_key[(int)(iByte / 4)])  ^ mask;
  		state[iByte+2] = state[iByte+2] ^ BYTE3(rnd_key[(int)(iByte / 4)])  ^ mask;
  		state[iByte+3] = state[iByte+3] ^ BYTE4(rnd_key[(int)(iByte / 4)])  ^ mask;
  	}

  	for (iByte = 0; iByte < 16; iByte++)
  		ctext[iByte] = state[iByte];

}
