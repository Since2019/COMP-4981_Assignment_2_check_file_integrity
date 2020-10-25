#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "input_handler.h"

#define EVEN 0
#define ODD 1
#define READBITS 8

void encode(char c, int p_mode)
{

    // encoder
    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    FILE *fout = stdout;
    int8_t ch = c;

    int pass = 0;
    int p_check = 0;
    while (pass < READBITS)
    {
        if (ch & mask[7 - pass])
        {
            fprintf(fout, "%d", 1);
            p_check++;
        }
        else
        {
            fprintf(fout, "%d", 0);
        }
        pass++;
    }

    // append parities

    // --even
    if (p_mode == 0)
    {
        fprintf(fout, "%d", p_check % 2);
    }
    // --odd
    else if (p_mode == 1)
    {
        if (p_check % 2 != 0)
        {
            fprintf(fout, "%d", 0);
        }
        else
        {
            fprintf(fout, "%d", 1);
        }
    }
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

    // read input
    if (parse_args_wopt(argc, argv, long_options))
    {
        return 1; // error: file input
    }

    char c;

    while (1)
    {
        c = getchar();
        if (c == EOF)
        {
            break;
        }

        encode(c, e_mode);
    }

    return 0;
}