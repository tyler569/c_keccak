
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sponge.h"

char *test_permutation(char *state) {
    char tmp = state[0];
    state[0] = state[1] + 1;
    state[1] = tmp ^ 0xAA;
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

    sponge_update(s, "Test", 4);
    sponge_update(s, "Other Tests", 11);
    char *result = sponge_digest(s, 16);
    for (int i=0; i<16; i+=1) {
        printf("%x ", *(result + i));
    }
    printf("\n");
}
