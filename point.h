#ifndef _POINT
#define _POINT

#include <gmp.h>

typedef struct _point {
    mpz_t x;
    mpz_t y;
} Point;

extern Point* point_create(const char*, const char*);
extern Point* point_create_mpz_t(const mpz_t, const mpz_t);
extern Point* point_at_infinity();
extern int point_is_equal(const Point*, const Point*);
extern Point* point_copy(const Point*);
extern void point_destroy(Point*);

#endif
