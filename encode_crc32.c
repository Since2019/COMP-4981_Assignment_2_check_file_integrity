#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "input_handler.h"

/**
  * Converts decimal to binary ( in char array )
  * @param decimal_num : the number to convert
  * @param bin_8_bits :  the result stored in char arry.
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

/** Converts crc to Binary (in char array)
 * @param decimal_num: the number to convert from
 * @param bin_32_bits: the variable to store the binary result of conversion in char array.
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
    Generate CRC32 byte by byte
    @param chars: the char array to store crc
    @param num_chars: counts the number of chars
*/
uint32_t crc32(char *chars, int num_chars)
{
    const uint32_t poly = 0x04C11DB7;
    uint32_t crc = 0;

    int i = 0;
    for (i = 0; i < num_chars; i++)
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
    calculates the crc
    @param c: is the char that read in.
    @param crc : takes in previous crc.
*/
uint32_t calc_crc(char c, uint32_t crc)
{
    const uint32_t poly = 0x04C11DB7;

    int i = 0;

    crc ^= (uint32_t)(c << 24);
    for (i = 0; i < 8; i++)
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

    if(parse_args(argc, argv))
    {
        return -1;
    }

    uint32_t crc = 0;

    char *bin_8_bit = malloc(sizeof(char) * 9);
    char *bin_32_bit = malloc(sizeof(char) * 33);

    int c;
    while(1)
    {
        c = getchar();

        if(c == EOF)
            break;

        crc = calc_crc(c, crc);
        decimalToBinary(c, bin_8_bit);
        fprintf(stdout, "%s", bin_8_bit);
    }

    crcToBinary(crc, bin_32_bit);
    fprintf(stdout, "%s", bin_32_bit);

    return 0;
}
