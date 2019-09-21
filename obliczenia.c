#include "obliczenia.h"
#include <stdlib.h>
#include <stdbool.h>

bool compare(bignum a, bignum b, int pnt);
bool greater(bignum a, bignum b);

bignum add(bignum a, bignum b){
	if(a.sign == 0 && b.sign == 1){
		b.sign = 0;
		return sub(a, b);
	}
	if(a.sign == 1 && b.sign == 0){
		a.sign = 0;
		return sub(b, a);
	}

	bignum result;
	if(a.sign == 1 && b.sign == 1)
		result.sign = 1;
	if(a.sign == 0 && b.sign == 0)
		result.sign = 0;
	uint8_t *t;
	uint8_t buffer=0;

	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	result.size=0;
	while(result.size<a.size || result.size<b.size){
		if(result.size<a.size && (a.mem[result.size/8] & 1<<(result.size%8)) )
			buffer++;
		if(result.size<b.size && (b.mem[result.size/8] & 1<<(result.size%8)) )
			buffer++;
		t[result.size/8] += (buffer%2)<<(result.size%8);
		buffer /= 2;
		result.size++;
	}
	if(buffer){
		t[result.size/8] += buffer<<(result.size%8);
		result.size++;
	}
	for(int i=0; i<MAXSIZE;i++)
		if(t[i/8]&1<<(i%8))
			result.size=i+1;
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++)
		result.mem[i]=t[i];
	free(t);
	return result;
}

bignum sub(bignum a, bignum b){
	bignum result;
	result.sign=0;
	result.size=0;
	if(a.sign == 0 && b.sign == 1){
		b.sign = 0;
		return add(a, b);
	}
	if(a.sign == 1 && b.sign == 0){
		b.sign = 1;
		return add(a, b);
	}
	if(greater(b, a)==true){
		result = sub(b, a);
		if(a.sign == 1)
			result.sign = 0;
		else
			result.sign = 1;
		return result;
	}
	uint8_t *t;
	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	for(int i=0; i<a.size; i++){
		if((i<b.size) && !(a.mem[i/8] & (1<<(i%8))) && (b.mem[i/8] & 1<<(i%8)) ){
			int j=i;
			while(!(a.mem[j/8] & 1<<(j%8))){
				a.mem[j/8]+=(1<<(j%8));
				j++;
			}
			a.mem[j/8]-=1<<(j%8);
			b.mem[i/8]-=1<<(i%8);
		}
		if(i<b.size)
			t[i/8]+=((a.mem[i/8] & 1<<(i%8)) - (b.mem[i/8] & 1<<(i%8)));
		else
			t[i/8]+=(a.mem[i/8] & 1<<(i%8));
	}
	for(int i=0; i<MAXSIZE;i++)
		if(t[i/8]&1<<(i%8))
			result.size=i+1;
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++){
		result.mem[i]=t[i];
	}
	free(t);
	return result;
}

bignum mult(bignum a, bignum b){
	bignum result;
	result.size=0;
	if(a.sign == b.sign)
		result.sign = 0;
	else
		result.sign = 1;
	uint8_t *t;
	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	uint8_t buffer=0;
	for(int shift=0; shift<b.size; shift++)
		if(b.mem[shift/8]&1<<(shift%8)){
			for(int i=0; i<a.size; i++){
				if(t[(i+shift)/8] & 1<<((i+shift)%8)){
					buffer++;
					t[(i+shift)/8]-=(1<<((i+shift)%8));
				}
				if(a.mem[i/8] & 1<<(i%8) )
					buffer++;
				t[(i+shift)/8] += (buffer%2)<<((i+shift)%8);
				buffer /= 2;
			}
			if(buffer){
				t[(a.size+shift)/8] += buffer<<((a.size+shift)%8);
				buffer = 0;
			}
		}
	for(int i=0; i<MAXSIZE;i++)
		if(t[i/8]&1<<(i%8))
			result.size=i+1;
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++)
		result.mem[i]=t[i];
	free(t);
	return result;
}

bignum divi(bignum a, bignum b){
	bignum result;
	if(a.sign == b.sign)
		result.sign = 0;
	else
		result.sign = 1;
	if(a.size<b.size){ //return 0
		result.sign=0;
		result.size=0;
		result.mem = calloc(1, sizeof(uint8_t));
		result.mem[0]=0;
		return result;
	}

	uint8_t *t;
	t = calloc(MAXSIZE/8, sizeof(uint8_t));
	int pnt= a.size-1;
	while(pnt>=b.size){
		if(!(a.mem[pnt/8]&(1<<(pnt%8)))){
			pnt--;
			continue;
		}
		int x;
		if(compare(a, b, pnt)){
			x=(pnt-b.size+1);
		}
		else{
			x=(pnt-b.size);
		}
		t[x/8]+=1<<(x%8);
		for(int i=x; i<=pnt; i++){
			if(!(a.mem[i/8] & 1<<(i%8)) && (b.mem[(i-x)/8] & 1<<((i-x)%8))){
				int j=i;
				while(!(a.mem[j/8] & 1<<(j%8))){
					a.mem[j/8]+=(1<<(j%8));
					j++;
				}
				a.mem[j/8]-=(1<<j%8);
			}
			else if ((b.mem[(i-x)/8] & 1<<(i-x)%8))
				a.mem[i/8] -= 1<<(i%8);
		}
		pnt--;
	}
	if((a.mem[(b.size-1)/8]&(1<<((b.size-1)%8)))  && compare(a, b, pnt))
		t[0]+=1;

	for(int i=0; i<MAXSIZE;i++)
		if(t[i/8]&1<<(i%8))
			result.size=i+1;
	result.mem = calloc(result.size/8 +1, sizeof(uint8_t));
	for(int i=0; i<=result.size/8; i++)
		result.mem[i]=t[i];
	free(t);
	return result;
}

bool compare(bignum a, bignum b, int pnt){
	for(int i=b.size-1; i>=0 && pnt>=0; i--){
			if(((a.mem[pnt/8]&(1<<pnt%8)) && !(b.mem[i/8]&(1<<i%8))) ||
				(!(a.mem[pnt/8]&(1<<pnt%8)) && (b.mem[i/8]&(1<<i%8)))){
				if(a.mem[pnt/8]&(1<<pnt%8))
					return true;
				else
					return false;
			}
			pnt--;
	}
	return true;
}

bool greater(bignum a, bignum b){
	if(a.size>b.size)
		return true;
	if(b.size>a.size)
		return false;
	for(int i=a.size-1; i>=0; i--)
		if((a.mem[i/8]&(1<<i%8)) != (b.mem[i/8]&(1<<i%8))){
			if(a.mem[i/8]&(1<<i%8))
				return true;
			else
				return false;
		}
	return false;
}
