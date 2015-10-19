
/*
 * SHA3 Library
 * <License + me>
 */

#ifndef _SHA3_H_
#define _SHA3_H_

typedef struct sha3_ {
	int digest_len;
	/* state? */
} SHA3;


int sha3_224_init(char *type, SHA3 *c);
int sha3_224_update(SHA3 *c, const void *data, size_t len);
int sha3_224_final(SHA3 *c, unsigned char *md);

int sha3_256_init(char *type, SHA3 *c);
int sha3_256_update(SHA3 *c, const void *data, size_t len);
int sha3_256_final(SHA3 *c, unsigned char *md);

int sha3_384_init(char *type, SHA3 *c);
int sha3_384_update(SHA3 *c, const void *data, size_t len);
int sha3_384_final(SHA3 *c, unsigned char *md);

int sha3_512_init(char *type, SHA3 *c);
int sha3_512_update(SHA3 *c, const void *data, size_t len);
int sha3_512_final(SHA3 *c, unsigned char *md);

#endif
