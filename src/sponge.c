
/* 
 * An Implementation of the Keccak and SHA3 standard as
 * defined in FIPS-202.
 *
 * http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf
 *
 * Copyright (c) 2015, Tyler Philbrick
 * All rights reserved
 * See COPYING for license details
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sponge.h"

/*
 * sponge_init makes and initalizes the sponge contruction struct
 *
 * Inputs:
 * The various components of the struct
 */
Sponge *sponge_init(
        char *(*permutation_function)(char *state),
        char *(*padding_function)(char *state, size_t len, size_t final_len),
        size_t state_width,
        size_t rate) {
    Sponge *s = malloc(sizeof(Sponge));
    s->permutation_function = permutation_function;
    s->padding_function = padding_function;
    s->rate = rate;
    s->buffer = malloc(rate);
    s->state = malloc(state_width);
    s->input_length = 0;
    return s;
}

/*
 * sponge_free frees memory inside the Sponge struct, then the struct itself.
 * Simply free()'ing the Sponge pointer returned by sponge_init will
 * cause memory to leak.
 *
 * Inputs:
 * Sponge *s - the sponge to free
 */
void sponge_free(Sponge *s) {
    free(s->state);
    free(s->buffer);
    free(s);
}

/*
 * sponge_update takes input and absorbes it into the sponge
 * construction.
 *
 * Inputs:
 * Sponge *s - the sponge to absorb into
 * char *data - the data to absord
 * size_t data_len - the length of the data
 *
 * This function first copys any left-over bytes from the sponge
 * buffer, then fills the input from data and runs the
 * permutation function.  At the end, if there are odd remaining bits,
 * it places them back in the sponge buffer.
 */
void sponge_update(Sponge *s, char *data, size_t data_len) {
    char input[s->rate];
    size_t input_len = s->input_length % s->rate;
    int data_consumed = 0;
    size_t data_remaining = data_len;

    memcpy(input, s->buffer, input_len);

    while (data_consumed < data_len) {
        if (data_remaining < s->rate) {
            memcpy(s->buffer, data+data_consumed, data_remaining);
            data_consumed += data_remaining;
            s->input_length += data_consumed;
            return;
        }
        memcpy(input, data+data_consumed, s->rate-(input_len % s->rate));
        data_consumed += s->rate-(input_len % s->rate);
        input_len += s->rate-(input_len % s->rate);
        data_remaining = data_len - data_consumed;

        for (size_t i=0; i<s->rate; i+=1) {
            s->state[i] ^= input[i];
        }
        // It should be noted, this construction assumes 
        // s->permutation_function is freeing the old state.
        s->state = s->permutation_function(s->state);
    }
}

/*
 * sponge_digest runs the sponge padding function and outputs the
 * sponge function digest
 *
 * Inputs:
 * Sponge *s - the sponge to digest
 * size_t digest_len - the length of output to output
 *
 */
char *sponge_digest(Sponge *s, size_t digest_len) {
    char *input = malloc(s->rate);
    size_t input_len = s->input_length % s->rate;
    
    memcpy(input, s->buffer, input_len);
        
    input = s->padding_function(input, input_len, s->rate);
    if (input_len == 0 && input == NULL) {
        // Padding function signaled no padding is needed.
    } else {
        for (size_t i=0; i<s->rate; i+=1) {
            s->state[i] ^= input[i];
        }
        s->state = s->permutation_function(s->state);
        free(input);
    }
    
    // consuming last block
    // ------
    // squeezing
    
    char *digest = malloc(digest_len + (digest_len % s->rate));
    // The extra digest_len % s->rate is because we add data in a
    // s->rate at a time.
    size_t cur_digest_len = 0;

    while (cur_digest_len < digest_len) {
        memcpy(digest+cur_digest_len, s->state, s->rate);
        s->state = s->permutation_function(s->state);
        cur_digest_len += s->rate;
    }
    if (cur_digest_len > digest_len) {
        // truncate output?
        // it shouldn't tecnically make a difference, right?
    }
    return digest;
}

