#ifndef STRUKTURY_H
#define STRUKTURY_H

#include <stdbool.h>
#include <stdlib.h>

#include "bignum.h"

typedef struct Token{
	bool isBignum;
	bignum b;
	char c;
} token;

typedef struct List* pnode;
typedef struct List{
	token value;
	pnode left;
	pnode right;
} snode;

void push(token val, pnode* node);
token get_top(pnode* node);
token get_front(pnode* node);
token top(pnode node);
pnode find_front(pnode node);

#endif
