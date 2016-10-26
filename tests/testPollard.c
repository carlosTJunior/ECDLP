#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "unittests.h"
#include "../pollardrho.h"


void testCount_1bits() {
    printf("testCount_1bits ");

    assert(count_1bits(4) == 1);
    assert(count_1bits(5) == 2);
    assert(count_1bits(15) == 4);
    assert(count_1bits(3) == 2);

    assert(count_1bits(155) == 5);
    assert(count_1bits(1555) == 5);
    assert(count_1bits(2577) == 4);
    assert(count_1bits(2345789) == 13);
}


test_func pollardrho_functions[] = { 
    testCount_1bits,
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
