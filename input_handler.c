#include "input_handler.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */

int parse_args(int argc, const char* argv[])
{   

    // check if input file exists
    int arg_count = 1;
    while(arg_count < argc && strcmp(argv[arg_count], ">") != 0)
    {
        fprintf(stderr, "checking args at %d with %s\n", arg_count, argv[arg_count]);
        if(strstr(argv[arg_count], ".txt") != NULL)
        {
            
            int fd = open(argv[arg_count], O_RDONLY);
            dup2(fd, STDIN_FILENO);
            // close(fd);
        }
        
        if(strcmp(argv[arg_count], ">") == 0)
        {
            fprintf(stderr, "if file was defined, please use a txt file\n");
        }
        arg_count++;
    }

    return 0;
}

int parse_args_wopt(int argc, const char* argv[],struct option long_option[])
{   

    char* const* args = (char* const*)argv;

    // if arguments is less than 2
    if(argc < 2) {
        fprintf(stderr, "run with   ./program --even | --odd \n");
        return 1;
    }

    // option counter check
    int opt_count;
    while (1)
    {

        int option_index = 0;
        opt_count = getopt_long_only(argc, args, ":", long_option, &option_index);

        if (opt_count == -1)
            break;
    }

    // check if input exits
    int arg_count = 1;
    while(arg_count < argc && strcmp(argv[arg_count], ">") != 0)
    {
        fprintf(stderr, "checking args at %d with %s\n", arg_count, argv[arg_count]);
        if(strstr(argv[arg_count], ".txt") != NULL)
        {
            int fd = open(argv[arg_count], O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        arg_count++;
    }


    return 0;
}

void parse_file_args(int argc, const char* argv[], FILE* ifile)
{   

    // check if input file exists
    int arg_count = 1;
    while(arg_count < argc && strcmp(argv[arg_count], ">") != 0)
    {
        fprintf(stderr, "checking args at %d with %s\n", arg_count, argv[arg_count]);
        if(strstr(argv[arg_count], ".txt") != NULL)
        {
            ifile = fopen(argv[arg_count], "r");
            int fd = fileno(ifile);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        

        if(strcmp(argv[arg_count], ">") == 0)
        {
            fprintf(stderr, "if file was defined, please use a txt file\n");
        }
        arg_count++;
    }
}
