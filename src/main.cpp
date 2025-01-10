#include <string.h>
#include "include/processor.h"

int main(int argc, char** argv) {
    Processor processor;
    processor.Run(argc > 1 && strcmp(argv[1], "-v") == 0);
    return 0;
}