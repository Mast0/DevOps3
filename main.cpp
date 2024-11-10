#include <iostream>
#include "arctan.h"

int main() {
    double x = 0.5;
    int terms = 10;
    std::cout << "arctan(" << x << ") ≈ " << arctan(x, terms) << std::endl;
    return 0;
}
