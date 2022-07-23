#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zconf.h>
#include <sys/stat.h>

/**
 * C EXCEPTION LIBRARY
 * By Yochran
 */

#include "exceptions.h"

/**
 * FORMAT:
 * universify <program/script>
 */

/**
 * @brief Main function
 * 
 * @param argc - argument count
 * @param argv - argument array
 * @return int 
 */
int main(int argc, char *argv[])
{
    // get euid
    uid_t uid = getuid(), euid = geteuid();

    // check if running as root
    if (uid != euid || uid != 0)
    {
        // create exception
        exception _exception = {
            "You must be running as a super-user to use universify.",
            NO_PERMISSION
        };

        // throw exception
        _throw(&_exception);
        exit(0);
    }

    // check argument length
    if (argc != 2)
    {
        // create exception
        exception _exception = {
            "Correct usage: (universify <program/script>)",
            INCORRECT_USAGE
        };

        // throw exception
        _throw(&_exception);
        exit(0);
    }

    // create program variable
    char *program = (char*) argv[1];

    // check if program exists
    struct stat buffer;

    if (stat(program, &buffer) != 0)
    {
        // create exception
        exception _exception = {
            "Error: Cannot find program with that name.",
            FILE_NOT_FOUND
        };

        // throw exception
        _throw(&_exception);
        exit(0);
    }

    // check if attempt to access outside of immediate directory
    if (strstr(program, "/"))
    {
        // create exception
        exception _exception = {
            "You cannot access a program from outside of it's directory.",
            ATTEMPT_REMOTE_ACCESS
        };

        // throw exception
        _throw(&_exception);
        exit(0);

        // free memory
        free(program);
    }

    // get directory to copy to
    char *dir = "/usr/bin";

    // create command
    char *command = (char*) malloc(100);
    sprintf(command, "cp %s %s/%s", program, dir, program);

    // run command
    system(command);

    // log
    printf("You have made \"%s\" a universal program.\n", program);

    // free memory
    free(command);

    return 0;
}
