#include "common/debug.hpp"

static hotel::debug::debug_streambuf sb;
std::ostream hotel::debug::out(&sb);

void dummy() {
    std::cout << "hi?" << std::endl;
}
