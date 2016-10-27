#ifndef _MERSENNE_TWISTER
#define _MERSENNE_TWISTER


extern void init_genrand64(long long seed);
extern void init_by_array64(long long init_key[],
		             long long key_length);

extern long long genrand64_int64(void);
extern long long genrand64_int63(void);

#endif
