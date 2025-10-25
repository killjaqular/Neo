// 3rd Party Libraries

// Platform Libraries

// Project Libraries
#include "include/logger.hpp"

// Standard Libraries
#include <cstdbool>
#include <cstdlib>
#include <string.h>

const char USAGE[] = {
"\nUsage: program [-help]\n"
"    -help...........................Display this help menu.\n"
};

bool parse_cli(int argc, char * argv[]) {
    bool ret = false;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            goto help;

        } else {
            fprintf(stdout, "Unknown option=<%s>\n", argv[i]);
            goto unknown_option;
        }
    }

unknown_option:
help:
    return ret;
}

int main(int argc, char * argv[]) {
    int ret = EXIT_FAILURE;

    if (!parse_cli(argc - 1, argv + 1)) {
        puts(USAGE);
        goto parse_cli_fail;
    }

    INFO(stdout, "Starting...");

    ret = EXIT_SUCCESS;
    INFO(stdout, "Stopping...");

parse_cli_fail:
    return ret;
}
