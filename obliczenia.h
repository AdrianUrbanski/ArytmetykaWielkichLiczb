#ifndef OBLICZENIA_H
#define OBLICZENIA_H
#define MAXSIZE 4096
#define MAXSIZEDEC 1234

#include "bignum.h"

bignum add(bignum a, bignum b);
bignum sub(bignum a, bignum b);
bignum mult(bignum a, bignum b);
bignum divi(bignum a, bignum b);

#endif
