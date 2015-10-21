
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
    Sponge *s = sponge_init(&test_permutation, &test_padding, 2, 1);
    Sponge *t = sponge_init(&test_permutation, &test_padding, 2, 1);
    
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

