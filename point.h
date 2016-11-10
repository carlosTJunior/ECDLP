#ifndef _POINT
#define _POINT

#ifdef _LIB_GMP
#include <gmpxx.h>
#define BigInt mpz_class
#define STR(a) ((a).get_str(10).c_str())
#define UINT(a, b) ((a).get_ui())

#else
#include <ttmath/ttmath.h>
#define BigInt ttmath::Int<12>
#define STR(a) ((a).ToString().c_str())
#define UINT(a, b) ((a).ToUInt((b)))

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
