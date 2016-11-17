#ifndef _POINT
#define _POINT

#ifdef _LIB_GMP
#include <gmpxx.h>
#define BigInt mpz_class
#define STR(a) ((a).get_str(10).c_str())

#else
#include <ttmath/ttmath.h>
#define BigInt ttmath::Int<12>
#define STR(a) ((a).ToString().c_str())

#endif

struct Point {
    BigInt x;
    BigInt y;

    Point() {}
    ~Point() {}
    Point(const BigInt x, const BigInt y) : x(x), y(y) {}
    friend void point_at_infinity(Point& P);
    friend bool operator==(const Point P, const Point Q);
};

#endif
