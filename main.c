// This is an example of a very simple program which used the cbinem library.

// It accepts file paths as arguments and processes each one to generate C
// source files which can be later embedded in to a target application.

#define BYTES_PER_LINE 64
#define ENABLE_PRETTY_FORMATTING 1

#include "cbinem.h"

int main(int argc, char** argv) {
    char** arg = argv + 1;
    for (; arg != argv + argc; arg++)
        cbinem_generate_from_file(*arg, BYTES_PER_LINE, ENABLE_PRETTY_FORMATTING);
    return 0;
}
