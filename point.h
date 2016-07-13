#ifndef _POINT
#define _POINT

typedef long BigInt;

typedef struct _point {
    BigInt x;
    BigInt y;
} Point;

extern Point point_create(BigInt, BigInt);

#endif
