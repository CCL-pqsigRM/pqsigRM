// common.c for pqsigRM
#include "common.h"

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long sign_i){
	// Assuming 64bit environment, say size_t is defined as unsigned long
	// Hash the given message
	// syndrome s = h(h(M)|i)|(h(h(M)|i)|i)
	SHA512(m, mlen, s);
	*(unsigned long long*)(s+HASHSIZEBYTES) = sign_i;// concatenate i i.e. h(h(M)|i)
	SHA512(s, HASHSIZEBYTES+sizeof(unsigned long long), s); //h(h(M)|i)
	
	size_t idx;
	for(idx=1; idx < 8; ++idx)
		SHA512(s+HASHSIZEBYTES*(idx-1), HASHSIZEBYTES, s+HASHSIZEBYTES*idx);//(h(h(M)|i)|i)
	
	return s;
}

unsigned int hammingWgt(unsigned char* e, size_t elen){
	unsigned int w=0;
	int i;
	for(i=0; i<elen; ++i)
		w+=e[i];
	return w;
}
