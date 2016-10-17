#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"

int main() {
    BigInt r;

    r = calculate_result(192, 213, 104, 24, 239);

    printf("%ld\n", r);

    return 0;
}
