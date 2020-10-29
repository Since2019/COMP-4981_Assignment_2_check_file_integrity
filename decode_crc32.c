#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "input_handler.h"

/**
    A function to conver decimal int to binary in char*
    @param decimal_num: the decimal number you want you convert into Binary
    @param bin_8_bits: char array that would store the 8 bits Binary you converted to
*/
void decimalToBinary(int decimal_num, char *bin_8_bits)
{
    int c, result;

    for (c = 7; c >= 0; c--)
    {
        result = decimal_num >> c;

        if (result & 1)
            bin_8_bits[7 - c] = '1';
        else
            bin_8_bits[7 - c] = '0';
    }
    bin_8_bits[8] = '\0';
}

/**
   changes the CRC (32 bits) into Binary 
   @param decimal_num: is the decimal CRC 
   @param bin_32_bits: the 32-bit output in char*
*/
void crcToBinary(int decimal_num, char *bin_32_bits)
{
    int c, result;

    for (c = 31; c >= 0; c--)
    {
        result = decimal_num >> c;

        if (result & 1)
            bin_32_bits[31 - c] = '1';
        else
            bin_32_bits[31 - c] = '0';
    }
    bin_32_bits[32] = '\0';
}

/**
    Bitwise Shifting to create a unsigned integer(8 bits)
    @param *b : the pointer to the integer you want to deal with
    @param append_zero: specifies if this is divisible
*/
void append_to_int8(uint8_t *b, char append_zero)
{
    if (append_zero == '1') // appends 1
    {
        *b = ~*b;
        *b = *b << 1;
        *b = ~*b;
    }
    else if (append_zero == '0') // appends 0
    {
        *b = *b << 1;
    }
}

/**
    the function to create a crc array and put it into the char*
    @param chars :  goes to the 
    @param num_char : number of chars taken in
*/
uint32_t crc32(char *chars, int num_chars)
{
    const uint32_t poly = 0x04C11DB7;
    uint32_t crc = 0;

    // int i = 0;
    for (int i = 0; i < num_chars; i++)
    {
        crc ^= (uint32_t)(chars[i] << 24);
        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80000000) != 0) /* test for MSB = bit 31 */
            {
                crc = (uint32_t)((crc << 1) ^ poly);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

/**
    Get the remainder(crc) from bit shifting
    @param c: the char to read in.
    @param crc: take in the previous CRC
*/
uint32_t calc_crc(char c, uint32_t crc)
{
    const uint32_t poly = 0x04C11DB7;

    // int i = 0;

    crc ^= (uint32_t)(c << 24);
    for (int i = 0; i < 8; i++)
    {
        if ((crc & 0x80000000) != 0) /* test for MSB = bit 31 */
        {
            crc = (uint32_t)((crc << 1) ^ poly);
        }
        else
        {
            crc <<= 1;
        }
    }

    return crc;
}

int main(int argc, char const *argv[])
{

    // file pointeres for command line checker
    FILE *in_file = stdin;
    FILE *out_file = stdout;

    FILE *tmp_file = NULL;

    parse_file_args(argc, argv, in_file);

    uint32_t crc = 0;

    char c = 0;
    uint8_t byt = 0x00;
    int is_byte = 0;

    int num_bytes = 0;

    //  if tmp_file exists, writes to it.
    if (in_file == stdin)
    {
        tmp_file = fopen("decode_crc32.tmp", "w");
    }

    while (1)
    {
        c = getc(in_file);
        if (c == EOF || c == '\n')
            break;

        if (is_byte == 8)
        {
            crc = calc_crc(byt, crc);
            is_byte = 0;
            num_bytes++;
        }
        append_to_int8(&byt, c);
        fprintf(tmp_file, "%c", c);
        is_byte++;
    }
    fclose(tmp_file);

    crc = calc_crc(byt, crc);

    // CRC gets a remainder of 0
    if (crc == 0)
    {
        fprintf(stderr, "CRC checking passed\n");

        if (isatty(fileno(in_file)) != 1)
        {
            fseek(in_file, 0, SEEK_SET);
            clearerr(in_file);
        }

        num_bytes -= 4;
    }
    else
    {
        fprintf(stderr, "CRC checking failed: ");
        return 1;
    }

    int temp_num_bytes = 0;
    is_byte = 0;

    //  keyboard input:
    FILE *read_tmp_file = fopen("decode_crc32.tmp", "r");

    if (read_tmp_file != NULL)
    {

        while (1)
        {
            c = getc(read_tmp_file);

            if (c == EOF)
            {
                break;
            }

            if (is_byte == 8 && temp_num_bytes <= num_bytes)
            {
                is_byte = 0;
                fprintf(out_file, "%c", byt);

                temp_num_bytes++;
            }
            append_to_int8(&byt, c);
            is_byte++;
        }
    }
    fclose(read_tmp_file);

    fclose(in_file);
    fclose(out_file);
    unlink("decode_crc32.tmp");

    return 0;
}
