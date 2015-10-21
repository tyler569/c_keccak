
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
        memcpy(input, data+data_consumed, s->rate-input_len);
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
    
    // I would check here to ensure the block is not full,
    // but that should be impossible as far as I know,
    // and if it's happening I have bigger problems.

    input = s->padding_function(input, input_len, s->rate);
    for (size_t i=0; i<s->rate; i+=1) {
        s->state[i] ^= input[i];
    }
    s->state = s->permutation_function(s->state);

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