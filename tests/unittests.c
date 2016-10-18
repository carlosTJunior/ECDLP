#include <stdio.h>
#include <stdlib.h>
#include "unittests.h"


int main() {
    numTests = 0;
    run_point_tests();
    run_ecc_tests();
    run_pollardrho_tests();

    printf("%d tests executed successfully\n", numTests);
    return 0;
}
