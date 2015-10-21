
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "sponge.h"

char *test_permutation(char *state) {
    (*state) += 1;
    return state;
}

char *test_padding(char *state, size_t len, size_t final_len) {
    return state;
}

int main() {
    Sponge *s = malloc(sizeof(Sponge));
    s->permutation_function = &test_permutation;
    s->padding_function = &test_padding;
    s->rate = 1;
    s->width = 1;
    s->state = malloc(1);
    s->buffer = malloc(1);
    s->input_length = 0;

    sponge_update(s, "Test", 4);
    sponge_update(s, "Test", 4);
    char *result = sponge_digest(s, 16);
    for (int i=0; i<16; i+=1) {
        printf("%x ", *(result + i));
    }
    printf("\n");
}
