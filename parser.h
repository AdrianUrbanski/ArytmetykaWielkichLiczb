#ifndef PARSER_H
#define PARSER_H

#include "konwersja.h"
#include "struktury.h"
#include "obliczenia.h"

#define MAXSIZEDEC 1234


pnode parseInput(char *s);
bignum evalRPN(pnode Qfront);

#endif
