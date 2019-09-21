#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
	char *s;
	s=malloc(1000);
	scanf("%s", s);
	print(evalRPN(parseInput(s)));
}
