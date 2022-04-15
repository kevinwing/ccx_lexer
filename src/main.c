#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

/**
 * @brief Program that takes in 1 or multiple files as arguments writes the
 *        contents into a new file, appending '.lexer.out' to the new file.
 * 
 * @param argc (int) Number of filenames to parse.
 * @param argv (char *) Array of filenames passed to program.
 * @return int Exit status of program.
 */
int main(int argc, char *argv[])
{
    // check for no arguments, if no filenames given exit with status 1.
    if (argc < 2)
    {
        fprintf(stderr, "Not enough arguments. exiting.");
        return 1;
    }

    // iterate over argv[] from 1 to end, calling parse on each given filename.
    for (int i = 1; i < argc; ++i)
    {
        parse(argv[i]);
    }

    return 0;
}
