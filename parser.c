#include "parser.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int precedence(char c);
bignum evaluate(char c, bignum a, bignum b);

pnode parseInput(char *s){
	int *t;
	int size;
	bignum b;
	token tok;
	t = calloc(MAXSIZEDEC, sizeof(int));
	pnode Qfront = NULL, Qend = NULL; //queue
	pnode Stop = NULL; //top of the stack
	while(*s){
		size=0;
		while(*s>='0' && *s<='9'){
			t[size]=*s-'0';
			size++;
			s++;
		}
		if(size){
			b = fromArray(t, size);
			tok.isBignum = true;
			tok.b=b;
			push(tok, &Qend);
		}
//		if(!(*s))
//			break;
		if(precedence(*s)){
			while(Stop!=NULL && precedence(top(Stop).c)>=precedence(*s))
				push(get_top(&Stop), &Qend);
			tok.isBignum=false;
			tok.b=fromInt(0);
			tok.c=*s;
			push(tok, &Stop);
		}
		if(*s=='('){
			tok.isBignum=false;
			tok.b=fromInt(0);
			tok.c=*s;
			push(tok, &Stop);
		}
		if(*s==')'){
			while(top(Stop).c!='('){
				push(get_top(&Stop), &Qend);
			}
			get_top(&Stop);
		}
		s++;
	}
	while(Stop!=NULL)
		push(get_top(&Stop), &Qend);

	Qfront=find_front(Qend);
	free(t);
	return Qfront;
}

bignum evalRPN(pnode Qfront){
	pnode Stop = NULL;
	token tok;
	while(Qfront!=NULL){
		tok = get_front(&Qfront);
		if(tok.isBignum==true)
			push(tok, &Stop);
		else{
			token o1 = get_top(&Stop);
			token o2 = get_top(&Stop);
			token b;
			b.b=evaluate(tok.c, o2.b, o1.b);
			push(b, &Stop);
			if(tok.isBignum)
				free((tok.b).mem);
			free((o1.b).mem);
			free((o2.b).mem);
		}
	}
	tok=get_top(&Stop);
	bignum result = tok.b;
	return result;
}

int precedence(char c){
	switch(c){
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return 0;
	}
}

bignum evaluate(char c, bignum a, bignum b){
	switch(c){
	case '+':
		return add(a,b);
	case '-':
		return sub(a, b);
	case '*':
		return mult(a, b);
	case '/':
		return divi(a, b);
	default:
		return a;
	}
}
