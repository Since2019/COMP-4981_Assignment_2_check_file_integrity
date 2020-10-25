#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "bit_handler.h"
#include "input_handler.h"

#define EVEN 0
#define ODD 1
#define READBITS 8

/**
    @param number the 8 bit number that will be printed
    @param file the file where the data is printed to
*/
void print_binary(int8_t number, FILE *file)
{
    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    int pass = 0;

    while (pass < READBITS)
    {
        if (number & mask[7 - pass])
        {
            fprintf(file, "%d", 1);
        }
        else
        {
            fprintf(file, "%d", 0);
        }
        pass++;
    }
}

/**
    @param chksum the chksum of the encoded data
    @param parity the parity bit for the chksum
    @param p_mode the parity mode for the chksum
*/
void print_chksum(int8_t chksum, int parity, int p_mode)
{
    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    FILE *fout = stdout;

    int pass = 0;
    int p_check = 0;
    if (p_mode == 0)
    {
        while (pass < READBITS)
        {
            if (chksum & mask[7 - pass])
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
        fprintf(fout, "%d", parity);
    }
    else
    {
        while (pass < READBITS)
        {
            if (chksum & mask[7 - pass])
            {
                fprintf(fout, "%d", 0);
            }
            else
            {
                p_check++;
                fprintf(fout, "%d", 1);
            }
            pass++;
        }
        fprintf(fout, "%d", !(parity));
    }
}

/**
    @param c the character to be encoded
    @param p_mode the parity mode
    @param chksum the current total of the bytes
    @param psum the current total of the parities
*/
void encode(char c, int p_mode, int8_t *chksum, int *psum)
{

    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    FILE *fout = stdout;
    int8_t ch = c;

    int pass = 0;
    int p_check = 0;

    *chksum = ch ^ *chksum;

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
        // ch = ch >> 1;
    }

    // --even
    if (p_mode == 0)
    {
        if (p_check % 2 == 0)
        {
            fprintf(fout, "%d", 0);
            *psum ^= 0x00;
        }
        else
        {
            fprintf(fout, "%d", 1);
            *psum ^= 0x01;
        }
    }
    // --odd
    else if (p_mode == 1)
    {
        if (p_check % 2 == 1)
        {
            fprintf(fout, "%d", 0);
            *psum ^= 0x00;
        }
        else
        {
            fprintf(fout, "%d", 1);
            *psum ^= 0x01;
        }
    }
}

int main(int argc, const char *argv[])
{

    // flags
    int e_mode = 2;

    // command arg options
    struct option long_options[] =
        {
            {"even", no_argument, &e_mode, EVEN},
            {"odd", no_argument, &e_mode, ODD},
            {0, 0, 0, 0}};

    if(parse_args_wopt(argc, argv, long_options))
    {
        return 1;
    }

    // total chksum
    int8_t chksum;

    chksum = 0x00;

    // total for parity
    int psum = 0;

    // print encoding mode
    fprintf(stderr, "e_mode is: %d\n", (int)e_mode);

    if (e_mode == 2)
    {
        perror("no encoding mode stated");
        exit(EXIT_FAILURE);
    }

    char c;
    while (1)
    {
        c = getchar();
        if (c == EOF)
        {
            break;
        }

        // process c
        encode(c, e_mode, &chksum, &psum);
    }

    // append the chksum
    print_chksum(chksum, psum, e_mode);

    return 0;
}