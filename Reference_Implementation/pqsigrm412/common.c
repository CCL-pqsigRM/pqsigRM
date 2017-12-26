// common.c for pqsigRM
#include "common.h"

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long sign_i){
	// Hash the given message
	// syndrome s = h(h(M)|i) | (h(h(M)|i)|i) | ...

	SHA512(m, mlen, s);
	*(unsigned long long*)(s+HASHSIZEBYTES) = sign_i;// concatenate i i.e. h(M)|i
	SHA512(s, HASHSIZEBYTES+sizeof(unsigned long long), s); //h(h(M)|i)
	
	size_t idx;
	for(idx=1; idx < 7; ++idx)
		SHA512(s+HASHSIZEBYTES*(idx-1), HASHSIZEBYTES, s+HASHSIZEBYTES*idx);//(h(h(M)|i))
	
	return s;
}

int hammingWgt(matrix* error){
	int wgt=0;
	int i=0;
	for(i=0; i < error->cols; i++)
		wgt += getElement(error, 0, i);
	return wgt;
}
