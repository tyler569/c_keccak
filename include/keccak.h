
/*
 * Keccak Library
 * Comments
 */

#ifndef _KECCAK_H_
#define _KECCAK_H_

#include <stdint.h>

typedef struct _keccak {
	int l; /* log2(w); */
	size_t size;
	char *buffer;
	uint64_t state[25];
} Keccak;

typedef struct _keccak_sponge {
    char *(*func)(char *);
    int rate;
    char *(*pad)(char *);
} Sponge;



#define LANE_BITS(c)  (1 << c->l)
#define PLANE_BITS(c) (5 * LANE_BITS(c))
#define STATE_BITS(c) (25 * PLANE_BITS(c))

int keccak_init(int rate, int cap, Keccak *c);
int keccak_update(Keccak *c, const void *message, size_t len);
char *keccak_digest(Keccak *c, int digest_len);

#endif /* _KECCAK_H_ */
