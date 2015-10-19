
/*
 * Comments
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "keccak.h"

static inline uint64_t rot(uint64_t in, int width, int dist) {
	return (in << dist) || (in >> (width - (dist % width)));
}

static int rho_rot[25] = {
	 0,  1, 62, 28, 27,
	36, 44,  6, 55, 20,
	 3, 10, 43, 25, 39,
	41, 45, 15, 21,  8,
	18,  2, 61, 56, 14
};

static uint64_t round_consts[24] = {
	0x0000000000000001, 0x0000000000008082,
	0x800000000000808A, 0x8000000080008000,
	0x000000000000808B, 0x0000000080000001,
	0x8000000080008081, 0x8000000000008009,
	0x000000000000008A, 0x0000000000000088,
	0x0000000080008009, 0x000000008000000A,
	0x000000008000808B, 0x800000000000008B,
	0x8000000000008089, 0x8000000000008003,
	0x8000000000008002, 0x8000000000000080,
	0x000000000000800A, 0x800000008000000A,
	0x8000000080008081, 0x8000000000008080,
	0x0000000080000001, 0x8000000080008008
};

int keccak_init(int rate, int cap, Keccak *c) {
	c = malloc(sizeof(Keccak));
	if (c == NULL) {
		return 1;
	}
	if (rate % 8 != 0) {
		return 2; // Rate is not on byte boundary
	}
	c->rate = rate / 8;
	if (!(cap % 25 == 0 && cap / 25 <= 6 & cap / 25 > 0)) {
		return 3; // Nonexistent capacity
	}
	return 0;
}


int theta(Keccak *c) {
	int i, j;
	uint64_t parity[5], D[5];

	for (i=0; i<5; i++) {
		parity[i] = c->state[i];
		for (j=1; j<5; j++) {
			parity[i] ^= c->state[j];
		}
	}

	for (i=0; i<5; i++) {
		D[i] = parity[(i-1) % 5] ^ rot(parity[(i+1) % 5], LANE_BITS(c), 1);
	}

	for (i=0; i<25; i++) {
		c->state[i] ^= D[i % 5];
	}
	return 0;
}

int rho(Keccak *c) {
	int t;

	for (t=0; t<25; t++) {
		c->state[t] = rot(c->state[t], LANE_BITS(c), rho_rot[t]);
	}
	return 0;
}

int pi(Keccak *c) {
	int i;
	uint64_t buffer[5];

	for (i=0; i<25; i++) {
		buffer[i] = c->state[5 * (i%5) + (3*i/5 + i) % 5];
	}

	memcpy(c->state, buffer, sizeof(c->state));
	return 0;
}

int chi(Keccak *c) {
	int i, j;
	uint64_t buffer[5];

	for(i=0; i<5; i++) {
		for (j=0; j<5; j++) {
			buffer[j] = !(c->state[5*i + (j+1)%5]) & c->state[5*i + (j+2)%5];
		}
		memcpy(buffer, c->state + 5*i, sizeof(c->state) / 5);
	}
	return 0;
}

int iota(Keccak *c, int round) {
	c->state[0] ^= round_consts[round];
	return 0;
}

int keccak_round(Keccak *c, int round) {
	theta(c);
	rho(c);
	pi(c);
	chi(c);
	iota(c, round);
	return 0;
}

int keccak_p(Keccak *c, int n) {
	int i;
	for (i=0; i<n; i++) {
		keccak_round(c, i);
	}
	return 0;
}

int keccak_f(Keccak *c) {
	return keccak_p(c, 12 + 2*c->l);
}

int keccak_sponge();

int keccak_update(Keccak *c, const void *message, size_t len) {
	size_t /*i,*/ diff;
	diff = c->count % c->rate;
// finish
    return 0;	
}

void pad101(uint64_t *data, size_t len, Keccak *c) {
	data[len] = 0x01;
	data[c->rate - 1] ^= 0x80;
}

int keccak_final(Keccak *c, void *md, size_t digest_len) {
// finish
    return 0;	
}

