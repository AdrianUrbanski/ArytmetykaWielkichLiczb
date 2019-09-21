#include "konwersja.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BASE 10

#define toDec 0.30103 //log10(2)
// 1 + size*toDec
#define toBin 3.322 //log2(10)
// 1 + size*toBin

bignum fromInt(int a){
	bignum result;
	uint8_t *t;
	result.size=0;
	if(a<0){
		a*=(-1);
		result.sign=1;
	}
	else
		result.sign=0;
	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	while(a){
		if(result.size%8==0){
			t[result.size/8]=0;
		}
		if(a%2){
			t[result.size/8]+=1<<(result.size%8);
		}
		result.size++;
		a/=2;
	}
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++)
		result.mem[i]=t[i];
	free(t);
	return result;
}

int toInt(bignum a){
	int result=0;
	int temp=1;
	for(int i=0; i<a.size; i++){
		if(a.mem[i/8] & (1<<(i%8)) )
			result+=temp;
		temp*=2;
	}
	if(a.sign==1)
		result*=(-1);
	return result;
}

bignum fromArray(int* s, int size){
	bignum result;
	result.size=0;
	result.sign=0;
	uint8_t *t;
	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	int* a;
	a = calloc(size, sizeof(int));
	for(int i=0; i<size; i++){
		a[i]=s[size-i-1];
	}
	bool empty=false;
	while(empty==false){
		if(result.size%8==0){
			t[result.size/8]=0;
		}
		if(a[0]%2){
			t[result.size/8]+=1<<(result.size%8);
		}
		empty=true;
		for(int i=0; i<size; i++){
			if(i>0 && a[i]%2==1){
				a[i-1]+=BASE/2;
				empty=false;
			}
			a[i]/=2;
			if(a[i]>0) empty=false;
		}
		result.size++;
	}
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++)
		result.mem[i]=t[i];
	free(a);
	free(t);
	return result;
}

int *toArray(bignum a){
	int *result;
	int size=0;
	int * t;
	t=calloc(MAXSIZEDEC, sizeof(int));
	for(int i=a.size-1; i>=0; i--){
			for(int j=0; j<size; j++)
				t[j]*=2;
			for(int j=0; j<size; j++)
				if(t[j]>=10){
					t[j]-=10;
					t[j+1]++;
				}
			if(a.mem[i/8]&(1<<(i%8))){
				t[0]++;
			}
			if(t[size])
				size++;
	}
	result = malloc((size+1)*sizeof(int));
	for(int i=0; i<size; i++)
		result[i]=t[size-i-1];
	result[size]=-1;
	free(t);
	return result;
}

void print(bignum a){
	int size=0;
	int * t;
	t=calloc(MAXSIZEDEC, sizeof(int));
	for(int i=a.size-1; i>=0; i--){
			for(int j=0; j<size; j++)
				t[j]*=2;
			for(int j=0; j<size; j++)
				if(t[j]>=10){
					t[j]-=10;
					t[j+1]++;
				}
			if(a.mem[i/8]&(1<<(i%8))){
				t[0]++;
			}
			if(t[size])
				size++;
	}
	if(a.sign)
		printf("-");
	if(size==0)
		printf("0");
	for(int i=size-1; i>=0; i--)
		printf("%d", t[i]);
	free(t);
	return;
}
