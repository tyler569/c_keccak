
/*
 * Comments
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "keccak.h"

/*
 * keccak_pad101 is the padding function defined by
 * the keccak familly of sponge functions. It appends a 1
 * to the message, which is then followed by enough 0s to
 * fill the buffer, and finally another 1.
 *
 * Inputs:
 * char *buffer - the buffer to pad - IS FREED
 * size_t len - the current length of valid data in the buffer
 * size_t final_len - the lengh to pad to
 *
 * All values are in bytes; sub-byte padding is not supported by
 * this implementation.
 */
char *keccak_pad101(char *buffer, size_t len, size_t final_len) {
    char *out_buf = malloc(final_len);
    memcpy(out_buf, buffer, len);
    free(buffer);
    out_buf[len] |= 0x80;
    out_buf[final_len-1] |= 0x01;
    return out_buf;
}

