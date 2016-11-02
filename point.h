#ifndef _POINT
#define _POINT

#include <gmpxx.h>

//#define BigInt long long
#define BigInt mpz_class

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
