#include <stdio.h>
#include <stdint.h>

uint8_t s_box[16] = { 0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6 };

uint8_t const_key[15][16] = {
	{0,0,0,1,0,1,0,1,1,0,1,1,0,0,1,1},{0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0},
	{1,0,1,0,0,1,0,0,0,0,1,1,0,1,0,1},{0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1},
	{0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1},{1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0},{0,0,0,0,1,0,1,1,1,1,0,0,1,1,0,0},
	{1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,1},{0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0},
	{0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1},{0,0,1,0,0,0,1,0,1,0,0,0,1,1,1,0},
	{0,1,0,1,0,0,0,1,0,0,1,1,0,0,0,0},{1,1,1,1,1,0,0,0,1,1,0,0,1,0,1,0},
	{1,1,0,1,1,1,1,1,1,0,0,1,0,0,0,0}
};

void SubCell(uint8_t *state) {
	for (int i = 0; i <= 15; i++) {
		state[i] = s_box[state[i]];
	}
}

void ShuffleCell(uint8_t *state) {
	uint8_t tmp[16];
	tmp[0] = state[0], tmp[1] = state[10], tmp[2] = state[5], tmp[3] = state[15],
	tmp[4] = state[14], tmp[5] = state[4], tmp[6] = state[11], tmp[7] = state[1],
	tmp[8] = state[9], tmp[9] = state[3], tmp[10] = state[12], tmp[11] = state[6],
	tmp[12] = state[7], tmp[13] = state[13], tmp[14] = state[2], tmp[15] = state[8];

	for (int i = 0; i <= 15; i++) {
		state[i] = tmp[i];
	}
}//permuted

void MixColumn(uint8_t *state) {
	uint8_t tmp[16];
	for (int i = 0; i <= 3; i++) {
		tmp[4 * i + 0] = state[4 * i + 1] ^ state[4 * i + 2] ^ state[4 * i + 3];
		tmp[4 * i + 1] = state[4 * i + 0] ^ state[4 * i + 2] ^ state[4 * i + 3];
		tmp[4 * i + 2] = state[4 * i + 0] ^ state[4 * i + 1] ^ state[4 * i + 3];
		tmp[4 * i + 3] = state[4 * i + 0] ^ state[4 * i + 1] ^ state[4 * i + 2];
	}

	for (int i = 0; i <= 15; i++) {
		state[i] = tmp[i];
	}
}
/*
	0 1 1 1
M = 1 0 1 1
	1 1 0 1
	1 1 1 0

x0, x1, x2, x3 = M * (x0, x1, x2, x3), i = 0, 4, 8, 12
Therefore, x0 = x1 ^ x2 ^ x3, x1 = x0 ^ x2 ^ x3, ...
*/

void KeyAdd(int round, uint8_t *state, uint8_t *key) {
	if (round % 2 == 0) {
		for (int i = 0; i <= 15; i++) {
			state[i] = state[i] ^ key[i] ^ const_key[round][i];
		}
	}
	else {
		for (int i = 0; i <= 15; i++) {
			state[i] = state[i] ^ key[i + 16] ^ const_key[round][i];
		}
	}
}
/*
128-bit secret key K = K0 || K1
RKi = K(i mod 2) XOR ¥ái, 0 <= i <= 14
Note that, ¥ái = ¥âi, 0 <= i <= 14
*/

void Encrypt(uint8_t *state, uint8_t *key) {
	for (int i = 0; i <= 15; i++) {
		state[i] = state[i] ^ key[i] ^ key[i + 16];
	} //WK = K0 XOR K1, They are added bitwise to the LSB of every round key nibble

	for (int i = 0; i <= 14; i++) {
		SubCell(state);
		ShuffleCell(state);
		MixColumn(state);
		KeyAdd(i, state, key);
	}
	SubCell(state);
	for (int i = 0; i <= 15; i++) {
		state[i] = state[i] ^ key[i] ^ key[i + 16];
	}
}

int main() {
	uint8_t input[16] = { 0x4,0x2,0xc,0x2,0x0,0xf,0xd,0x3,0xb,0x5,0x8,0x6,0x8,0x7,0x9,0xe };
	uint8_t key[32] = { 0x6,0x8,0x7,0xd,0xe,0xd,0x3,0xb,0x3,0xc,0x8,0x5,0xb,0x3,0xf,0x3
	,0x5,0xb,0x1,0x0,0x0,0x9,0x8,0x6,0x3,0xe,0x2,0xa,0x8,0xc,0xb,0xf };

	Encrypt(input, key);

	for (int i = 0; i <= 15; i++) {
		printf("%x", input[i]);
	}
	printf("\n");
}