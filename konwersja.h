#ifndef KONWERSJA_H
#define KONWERSJA_H

#define MAXSIZE 4096
#define MAXSIZEDEC 1234

#include "bignum.h"

bignum fromInt(int a);
int toInt(bignum a);
bignum fromArray(int* a, int size);
int *toArray(bignum a);
void print(bignum a);

#endif
