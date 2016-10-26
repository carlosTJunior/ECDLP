#ifndef _MERSENNE_TWISTER
#define _MERSENNE_TWISTER


void init_by_array64(long long init_key[],
		             long long key_length);

long long genrand64_int64(void);
long long genrand64_int63(void);

#endif
