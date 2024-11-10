#include <cmath>
#include "arctan.h"

double arctan(double x, int terms) {
    double result = 0.0;
    for (int n = 0; n < terms; ++n) {
        double term = ((n % 2 == 0) ? 1 : -1) * (pow(x, 2 * n + 1) / (2 * n + 1));
        result += term;
    }
    return result;
}
