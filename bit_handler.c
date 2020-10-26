#include <stdio.h>
#include <stdlib.h>
#include "bit_handler.h"
#define BYTETOBIT 8 // number of bits to read

int count_one_bits(int8_t b)
{
    // array of bit masks
    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    int pass = 0;    // used to check how many bits has been passed through
    int p_check = 0; // used for parity checking

    while (pass < BYTETOBIT)
    {
        if (b & mask[7 - pass])
        {
            p_check++;
        }
        pass++;
    }

    return p_check;
}


void append_to_int32(int32_t *b, int append_zero)
{
    if (append_zero == 1) // appends 1
    {
        *b = ~*b;
        *b = *b << 1;
        *b = ~*b;
    }
    else if (append_zero == 0)  // appends 0
    {
        *b = *b << 1;
    }
}

void append_to_int8(int8_t *b, int append_value)
{
    if (append_value == 1)
    {
        *b = ~*b;
        *b = *b << 1;
        *b = ~*b;
    }
    else if (append_value == 0)
    {
        *b = *b << 1;
    }
}

int char_to_int(char c)
{
    if(c == '0')
    {
        return 0;
    }
    else if (c == '1')
    {
        return 1;
    }

    return -1;
}

void print_bin(int val, int length)
{
    int what_bit_testing = 0;

    while (what_bit_testing < length)
    {
        if (val & 0x01)
        {
            // printf("bit %d is 1\n", what_bit_testing);
            printf("1");
        }
        else
        {
            // printf("bit %d is 0\n", what_bit_testing);
            printf("0");
        }

        what_bit_testing++;
        val = val >> 1;
    }
}
