
#ifndef _SPONGE_H_
#define _SPONGE_H_

typedef struct _sponge {
    char *(*permutation_function)(char *state);
    char *(*padding_function)(char *state, size_t len, size_t final_len);
    size_t rate;
    size_t width;
    char *state;
    char *buffer;
    size_t input_length;
} Sponge;

void sponge_update(Sponge *s, char *data, size_t len);
char *sponge_digest(Sponge *s, size_t digest_len);

#endif // _SPONGE_H_

