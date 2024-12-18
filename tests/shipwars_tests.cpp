// пока тесты не получается хорошо написать, буду это делать позже.


// #include <gtest/gtest.h>
// #include "include/processor.h"

// TEST(CommandProcessorTests, PingCommand) {
//     CommandProcessor cp;
//     std::stringstream input("ping\n");
//     std::stringstream output;

//     std::streambuf* cinBuf = std::cin.rdbuf();
//     std::streambuf* coutBuf = std::cout.rdbuf();

//     std::cin.rdbuf(input.rdbuf());
//     std::cout.rdbuf(output.rdbuf());

//     cp.processCommand("ping");
//     std::cin.rdbuf(cinBuf);
//     std::cout.rdbuf(coutBuf);

//     EXPECT_EQ(output.str(), "pong\n");
// }