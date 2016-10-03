#ifndef _POINT
#define _POINT

#include <gmp.h>

typedef struct _point {
    mpz_t x;
    mpz_t y;
} Point;

extern Point* point_alloc();
extern int point_init(Point*, const char*, const char*);
extern int point_init_mpz_t(Point*, const mpz_t, const mpz_t);
extern int point_at_infinity(Point*);
extern int point_is_equal(const Point*, const Point*);
extern int point_copy(Point*, const Point*);
extern void point_destroy(Point*);

#endif
