#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
    parses the arguments if an input file is provided

    @param argc the number of arguments
    @param argv the arguments
*/
int parse_args(int argc, const char* argv[]);

/*
    parses the arguments for options and if an input file is provided

    @param argc the number of arguments
    @param argv the arguments
    @param long_option the array of options
*/
int parse_args_wopt(int argc,  const char* argv[], struct option long_option[]);

/*
    parses the arguments for an input file to be used for the program

    @param argc the number of arguments
    @param argv the arguments
    @param ifile the input file
*/
void parse_file_args(int argc, const char* argv[], FILE* ifile);
#endif
