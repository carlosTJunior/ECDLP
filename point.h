#ifndef _POINT
#define _POINT

//#include <gmp.h>

#define BigInt long long

typedef struct _point {
    BigInt x;
    BigInt y;
} Point;

extern Point* point_alloc();
extern int point_init(Point*, const BigInt, const BigInt);
extern int point_at_infinity(Point*);
extern int point_is_equal(const Point*, const Point*);
extern int point_copy(Point*, const Point*);
extern void point_destroy(Point*);

#endif
