#ifndef BIGNUM_H
#define BIGNUM_H

#define MAXSIZE 4096
#define MAXSIZEDEC 1234

#include <stdint.h>

typedef struct bignum{
	uint8_t sign; // znak: 0 - dodatnia, 1 - ujemna
	uint16_t size;
	uint8_t *mem;
} bignum;

#endif
