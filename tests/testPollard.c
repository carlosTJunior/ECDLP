#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "unittests.h"
#include "../pollardrho.h"


void testCount_0bits() {
    printf("testCount_0bits ");

    int n = 9;

    assert(count_0bits(n) == 2);
}

test_func pollardrho_functions[] = { 
    testCount_0bits,
    NULL 
};

void run_pollardrho_tests() {
    int i = 0;

    while(pollardrho_functions[i] != NULL) {
        (*pollardrho_functions[i])();
        printf("OK\n");
        i++;
        numTests++;
    }
}
