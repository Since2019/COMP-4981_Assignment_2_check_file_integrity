#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>

#include "input_handler.h"
#include "bit_handler.h"

#define EVEN 0
#define ODD 1
#define READBITS 8

void decode(int8_t byte, int p_mode, int parity)
{

    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    int pass = 0;
    int p_check = 0;
    while (pass < READBITS)
    {
        if (byte & mask[7 - pass])
        {
            p_check++;
        }
        pass++;
    }

    // --even
    if (p_mode == 0)
    {
        if (p_check % 2 != parity)
        {
            perror("parity failed");
        }
    }
    // --odd
    else if (p_mode == 1)
    {
        if (p_check % 2 == parity)
        {
            perror("parity failed");
        }
    }

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
        return 1; // error: file input
    }

    // decode

    char c;
    int reads = 0;
    int8_t byte = 0;
    int parity = 0;

    while (1)
    {
        // if read characters is 8, process code

        c = getchar();
        if (c == EOF || c == '\n')
        {
            break;
        }

        if (reads == 8)
        {
            parity = char_to_int(c);
        }
        else
        {
            append_to_int8(&byte, char_to_int(c));
        }
        reads++;

        if (reads == 9)
        {
            decode(byte, e_mode, parity);
            reads = 0;
        }
    }

    return 0;
}
