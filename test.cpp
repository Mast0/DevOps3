#include <iostream>
#include <cmath>
#include <cassert>
#include "arctan.h"

void test_arctan() {
    assert(std::abs(arctan(0.5, 20) - std::atan(0.5)) < 0.001);
    assert(std::abs(arctan(0.1, 20) - std::atan(0.1)) < 0.001);
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_arctan();
    return 0;
}
