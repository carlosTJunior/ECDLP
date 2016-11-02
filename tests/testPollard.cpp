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

void testPartitionFunction() {
    printf("testPartitionFunction ");
    Point P(177, 232);
    Point Q(671, 234);

    // Assume that L = 32
    assert(partition_function(P), 17);
    assert(partition_function(Q), 31);
}

void testRandomNumber() {
    printf("testRandomNumber ");
    BigInt maximum(723);
    BigInt result = random_number(maximum);

    assert(result >= 0 && result < maximum);
}

void testCalculateResult() {
    printf("testCalculateResult ");
    BigInt order(239), c1(192), d1(24), c2(213), d2(104);
    BigInt result = calculate_result(c1, c2, d1, d2, order);

    assert(result == 176);
}

test_func pollardrho_functions[] = { 
    testCount_1bits,
    testPartitionFunction,
    testRandomNumber,
    testCalculateResult,
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
