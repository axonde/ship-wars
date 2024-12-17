#include <iostream>
#include "include/kernel.h"
#include "include/processor.h"

int main() {
    std::cout << "Hello from Ship Wars!\n";
    Hello();
    Processor processor;
    processor.Run();
    return 0;
}