#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>

#include "bit_handler.h"
#include "input_handler.h"

#define EVEN 0
#define ODD 1

void decode(int8_t byte, int p_mode, int parity, uint8_t *total, int *psum, long int err_counter)
{

    uint8_t mode = 0x00;

    int p_check = 0; // used for parity checking

    // add current byte to total bytes
    *total = *total ^ byte;

    // add current parity to the parity sum for the checksum
    *psum = *psum ^ parity;

    p_check = count_one_bits(byte);

    // parity check
    if (p_mode == EVEN)
    {
        if (p_check % 2 != parity)
        {
            fprintf(stderr, "parity failed at byte %ld", err_counter);
        }
    }
    else if (p_mode == ODD)
    {
        if (p_check % 2 == parity)
        {
            fprintf(stderr, "parity failed at byte %ld", err_counter);
        }
    }

    if (p_mode == ODD)
    {
        mode = 0xFF;
    }

    if (*total != mode) // total of the whole encoded data is 0x00
        fprintf(stdout, "%c", (char)byte);
}

int main(int argc, char const *argv[])
{
    // flags
    int e_mode = 2;

    // command arg options
    struct option long_options[] =
        {
            {"even", no_argument, &e_mode, EVEN},
            {"odd", no_argument, &e_mode, ODD},
            {0, 0, 0, 0}};

    if (parse_args_wopt(argc, argv, long_options))
    {
        return 1;
    }

    // decode
    char c;
    int reads = 0;
    int8_t byte = 0;
    int parity = 0;

    // total chksum
    uint8_t chksum = 0x00;

    // total for parity
    int psum = 0;

    // error counter
    long int err_counter = 0;
    while (1)
    {
        // if read characters is 8, process code

        c = getchar();
        if (c == EOF)
        {
            break;
        }

        if (reads == 8) // current bit is the parity bit
        {
            parity = char_to_int(c);
        }
        else // current bit is not parity bit
        {
            append_to_int8(&byte, char_to_int(c));
        }
        reads++;

        if (reads == 9) // parity bit is reached
        {
            decode(byte, e_mode, parity, &chksum, &psum, err_counter);
            err_counter++;
            reads = 0;
        }
    }

    if (chksum == 0x00 && parity == 0)
    {
        perror("chksum passed");
    }

    return 0;
}