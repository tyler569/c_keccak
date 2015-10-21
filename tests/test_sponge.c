
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

void print_digest(char *digest, size_t len) {
    for (int i=0; i<len; i+=1) {
        printf("%x ", (unsigned char) *(digest + i));
    }
    printf("\n");
}

int main() {
    Sponge *s = sponge_init(&test_permutation, &test_padding, 2, 1);
    Sponge *t = sponge_init(&test_permutation, &test_padding, 2, 1);
    
    sponge_update(s, "1234", 4);
    sponge_update(s, "56789", 5);
    sponge_update(t, "123456789", 9);

    printf("Should be identical:\n");
    print_digest(sponge_digest(s, 16), 16);
    print_digest(sponge_digest(t, 16), 16);
    sponge_free(s);
    sponge_free(t);

    Sponge *zero_len = sponge_init(&test_permutation, &test_padding, 2, 1);

    printf("\n0 length input:\n");
    print_digest(sponge_digest(zero_len, 16), 16);
    
}

