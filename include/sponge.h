
#ifndef _SPONGE_H_
#define _SPONGE_H_

typedef struct _sponge {
    char *(*permutation_function)(char *);
    char *(*padding_function)(char *, size_t, size_t);
    size_t rate;
    char *buffer;
    char *state;
    size_t input_length;
} Sponge;

Sponge *sponge_init(
        char *(*permutation_function)(char *),
        char *(*padding_function)(char *, size_t, size_t),
        size_t, size_t);
void sponge_free(Sponge *s);
void sponge_update(Sponge *s, char *data, size_t len);
char *sponge_digest(Sponge *s, size_t digest_len);

#endif // _SPONGE_H_

