#include <stdio.h>
#include <stdlib.h>

#include"point.h"

#ifndef _LIB_GMP
#include <gmpxx.h>
#endif

#define GMPInt mpz_class

GMPInt maxgmp;

BigInt maxglb=0;

gmp_randclass r (gmp_randinit_mt);


void init_random_number_generator(const int seed, BigInt max)
{
   maxglb=max;
//    maxgmp.mpz_set_str("123");
    maxgmp.set_str(STR(max), 10);
//    mpz_set_str (&maxgmp, "1000", base);

    r.seed(seed);
}


BigInt random_number(const BigInt max)
{ 
    if (max != maxglb)
    {
      fprintf(stderr, "Erro max != maxglb\n");
      exit(1);
    }
    GMPInt temp = r.get_z_range(maxgmp);
    //printf("random = %s\n", temp.get_str(10).c_str());
#ifdef _LIB_GMP
    return temp;
#else
    BigInt tmp(temp.get_str(10).c_str());
//    printf("random = %s\n", STR(tmp));
    return tmp;
#endif
   
}

#ifdef _TEST_MAIN
int main(int argc, char *argv[])
{
#ifndef _LIB_GMP
  printf("Using ttmath\n");
#else
  printf("Using gmp\n");
#endif
  BigInt max=100;
  init_random_number_generator(0, max);
  
  for(int i=0; i<10; i++) printf("Random = %s\n", STR(random_number(max)));
  return 0;
}
#endif

