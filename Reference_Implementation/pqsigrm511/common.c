// common.c for pqsigRM
#include "common.h"

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long sign_i){
	// Hash the given message
	// syndrome s = h(h(M)|i) | (h(h(M)|i)) | ...

	*(unsigned long long*)(s+HASHSIZEBYTES) = sign_i;// concatenate i i.e. h(M)|i
	
	SHA512(m, mlen, s);
	SHA512(s, HASHSIZEBYTES+sizeof(unsigned long long), s); //h(h(M)|i)
	
	SHA512(s, HASHSIZEBYTES, s+HASHSIZEBYTES);//(h(h(M)|i))
	
	return s;
}

int hammingWgt(matrix* error){
	int wgt=0;
	int i=0;
	for(i=0; i < error->cols; i++)
		wgt += getElement(error, 0, i);
	return wgt;
}

void swap16(uint16_t *Q, const int i, const int j){
	uint16_t temp;
	temp = Q[i];
	Q[i] = Q[j];
	Q[j] = temp;
}

void permutation(uint16_t *Q, int len){
	int i,j; 
	for(i=0; i<len; i++)
		Q[i] = i;
	for(i=0; i<len; i++)
		swap16(Q, i, random16(len));
}

uint16_t random16(uint16_t n){
	uint16_t r;
	randombytes((unsigned char*)&r, 2);
	return r%n;
}

void col_permute(matrix* m, const int rf, const int rr, const int cf, const int cr, uint16_t* Q){
	matrix* mcpy = newMatrix(m->rows, m->cols); memcpy(mcpy->elem, m->elem, m->alloc_size);
	int r, c;
	for(c = cf; c < cr; c++)
		for(r = rf; r < rr; r++)
			setElement(m, r, c, getElement(mcpy, r, cf + Q[c-cf]));
	deleteMatrix(mcpy);
}

void y_permute(float *y, const int f, const int r, uint16_t *Q){
	int i; 
	float *ycpy = (float*)malloc(sizeof(float)*(r-f));
	for (i = 0; i < r - f; ++i)
		ycpy[i] = y[f + i];
	for (i = 0; i < r - f; ++i)
		y[f + i] = ycpy[Q[i]];
	free(ycpy);
}

void y_depermute(float *y, const int f, const int r, uint16_t *Q){
	int i; 
	float *ycpy = (float*) malloc(sizeof(float)*(r-f)); 
	for (i = 0; i < r - f; ++i)
		ycpy[i] = y[f + i];
	for (i = 0; i < r - f; ++i)
		y[f + Q[i]] = ycpy[i];
	free(ycpy);
}
