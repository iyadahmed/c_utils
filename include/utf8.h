// From: https://gist.github.com/tylerneylon/9773800

#pragma once

#include <assert.h>

#include "clz.h"

// Stops at any null characters.
int decode_code_point(char** s)
{
    int k = **s ? clz(~(**s << 24)) : 0; // Count # of leading 1 bits.
    int mask = (1 << (8 - k)) - 1; // All 1's with k leading 0's.
    int value = **s & mask;
    for (++(*s), --k; k > 0 && **s; --k, ++(*s)) { // Note that k = #total bytes, or 0.
        value <<= 6;
        value += (**s & 0x3F);
    }
    return value;
}

// Assumes that code is <= 0x10FFFF.
// Ensures that nothing will be written at or beyond end.
void encode_code_point(char** s, char* end, int code)
{
    assert(code >= 0 && code <= 0x10FFFF);
    char val[4];
    int lead_byte_max = 0x7F;
    int val_index = 0;
    while (code > lead_byte_max) {
        val[val_index++] = (code & 0x3F) | 0x80;
        code >>= 6;
        lead_byte_max >>= (val_index == 1 ? 2 : 1);
    }
    val[val_index++] = (code & lead_byte_max) | (~lead_byte_max << 1);
    while (val_index-- && *s < end) {
        **s = val[val_index];
        (*s)++;
    }
}

// Returns 0 if no split was needed.
int split_into_surrogates(int code, int* surr1, int* surr2)
{
    assert(code >= 0 && code <= 0x10FFFF);
    if (code <= 0xFFFF)
        return 0;
    *surr2 = 0xDC00 | (code & 0x3FF); // Save the low 10 bits.
    code >>= 10; // Drop the low 10 bits.
    *surr1 = 0xD800 | (code & 0x03F); // Save the next 6 bits.
    *surr1 |= ((code & 0x7C0) - 0x40) << 6; // Insert the last 5 bits less 1.
    return 1;
}

// Expects to be used in a loop and see all code points in *code. Start *old at 0;
// this function updates *old for you - don't change it. Returns 0 when *code is
// the 1st of a surrogate pair; otherwise use *code as the final code point.
int join_from_surrogates(int* old, int* code)
{
    if (*old)
        *code = (((*old & 0x3FF) + 0x40) << 10) + (*code & 0x3FF);
    *old = ((*code & 0xD800) == 0xD800 ? *code : 0);
    return !(*old);
}
