#ifndef BIT_HANDLER_H
#define BIT_HANDLER_H

#include <inttypes.h>
#include <stdbool.h>


/*
    @param b the binary string
*/
int count_one_bits(int8_t b);

/*
    @param b the binary string
    @param append_zero if 0 append 0 to b, else append 1
*/
void append_to_int32(int32_t *b, int append_zero);

/*
    @param b the binary string
    @param append_zero if 0 append 0 to b, else append 1
*/
void append_to_int8(int8_t *b, int append_zero);

/*
    @param c the character that can be '0' or '1'
*/
int char_to_int(char c);

/*
    @param b the binary string
    @param length the length of the binary string
*/
void print_bin(int b, int length);

#endif
