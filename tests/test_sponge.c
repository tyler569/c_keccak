
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sponge.h"

char *test_permutation(char *state) {
    char tmp = state[0];
    state[0] = state[1];
    state[1] = tmp;
    return state;
}

char *test_padding(char *buffer, size_t len, size_t final_len) {
    if (len < final_len) {
        char *out_buf = malloc(final_len);
        memcpy(out_buf, buffer, len);
        for (int i=len; i<final_len; i+=1) {
            out_buf[i] = 1;
        }
        free(buffer);
        return out_buf;
    }
    return buffer;
}

int main() {
    Sponge *s = malloc(sizeof(Sponge));
    s->permutation_function = &test_permutation;
    s->padding_function = &test_padding;
    s->rate = 1;
    s->width = 2;
    s->state = malloc(s->width);
    s->buffer = malloc(s->rate);
    s->input_length = 0;
    
    Sponge *t = malloc(sizeof(Sponge));
    t->permutation_function = &test_permutation;
    t->padding_function = &test_padding;
    t->rate = 1;
    t->width = 2;
    t->state = malloc(t->width);
    t->buffer = malloc(t->rate);
    t->input_length = 0;

    sponge_update(s, "1234", 4);
    sponge_update(s, "56789", 5);

    sponge_update(t, "123456789", 9);

    char *result_s = sponge_digest(s, 16);
    char *result_t = sponge_digest(t, 16);
    for (int i=0; i<16; i+=1) {
        printf("%x ", (unsigned char) *(result_s + i));
    }
    printf("\n");
    for (int i=0; i<16; i+=1) {
        printf("%x ", (unsigned char) *(result_t + i));
    }
    printf("\n");
}

