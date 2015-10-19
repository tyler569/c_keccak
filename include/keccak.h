
/*
 * Keccak Library
 * Comments
 */

#ifndef _KECCAK_H_
#define _KECCAK_H_

#include <stdint.h>

typedef struct keccak_ {
	int l; /* log2(w); */
	size_t rate;

	size_t count;
	void *buffer;

	uint64_t state[25];
} Keccak;

#define LANE_BITS(c)  (1 << c->l)
#define PLANE_BITS(c) (5 * LANE_BITS(c))
#define STATE_BITS(c) (25 * PLANE_BITS(c))

int keccak_init(int rate, int cap, Keccak *c);
int keccak_update(Keccak *c, const void *message, size_t len);
int keccak_final(Keccak *c, void *md, size_t digest_len);

#endif
