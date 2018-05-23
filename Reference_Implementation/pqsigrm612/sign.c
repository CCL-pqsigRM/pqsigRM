#include "api.h"
#include "rm.h"
#include "common.h"
#include "nearest_vector.h"
// #define SEEDEXPANDER_MAX_LEN 0xfffffffe // 2^32-1

int wgt(float *yc, float *yr)
{
	int i, w=0;
	for(i=0; i<CODE_N; i++)
		if(yc[i] != yr[i]) w++;
	return w;
}

matrix* syndromeForMsg(matrix* scrambled_synd_mtx, matrix *Sinv, matrix *synd_mtx, const unsigned char *m, 
						unsigned long long mlen, unsigned long long sign_i)
{
	hashMsg(synd_mtx->elem, m, mlen, sign_i);

	vector_mtx_product(scrambled_synd_mtx, Sinv, synd_mtx);
	return scrambled_synd_mtx;
}


void import_sk(const unsigned char *sk, matrix *Sinv
		, uint16_t **Q, uint16_t **Qp, uint16_t **s_lead)
{
	importMatrix(Sinv, sk);
	*Q 		= (uint16_t*)(sk+Sinv->alloc_size);
	*Qp 	= (uint16_t*)(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N);
	*s_lead = (uint16_t*)(sk+Sinv->alloc_size+sizeof(uint16_t)*CODE_N
							+sizeof(uint16_t)*CODE_N/4);
}

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
	const unsigned char *m, unsigned long long mlen,
	const unsigned char *sk){

	// read secret key(bit stream) into appropriate type.
	matrix* Sinv = newMatrix(CODE_N-CODE_K, CODE_N-CODE_K);
	uint16_t *Q, *Qp, *s_lead;
	import_sk(sk, Sinv, &Q, &Qp, &s_lead);
	// Do signing, decode until the a error vector wt <= w is achieved
	int i, j;
	
	unsigned long long sign_i;

	// unsigned char sign[CODE_N];
	matrix *synd_mtx= newMatrix(1, SYNDROMESIZEBYTES*8 );
	matrix *scrambled_synd_mtx = newMatrix(1, CODE_N - CODE_K);
	matrix *error = newMatrix(1, CODE_N);
	matrix *error_p = newMatrix(1, NUMOFPUNCTURE);

	float *yc = (float*)malloc(sizeof(float)*CODE_N);
	float *yr = (float*)malloc(sizeof(float)*CODE_N);
	
	init_decoding(CODE_N);
	while(1){
		//random number
		randombytes((unsigned char*)&sign_i, sizeof(unsigned long long));
		// Find syndrome
		syndromeForMsg(scrambled_synd_mtx, Sinv, synd_mtx, m, mlen, sign_i);

		// decode and find e
		// In the recursive decoding procedure,
		// Y is 1 when the received codeword is 0, o.w, -1
		for(i=0; i<CODE_N; i++) yr[i] = yc[i] = 1;
		
		for (i = 0; i < CODE_N - CODE_K; i++) 
			if(getElement(scrambled_synd_mtx, 0, i)== 1) 
				yr[s_lead[i]] = yc[s_lead[i]] = -1;

		recursive_decoding_mod(yc, RM_R, RM_M, 0, CODE_N, Qp);
		// for (int i = 0; i < CODE_N; ++i)
		// {
		// 	printf("%f \n", yc[i]);
		// }
		// Check Hamming weight of e'
		if(wgt(yr, yc) <= WEIGHT_PUB){
			break;
		}
	}
	// compute Qinv*e'
	matrix *sign = newMatrix(1, CODE_N);
	
	for(i=0; i<CODE_N; i++){
		setElement(sign, 0, Q[i], (yr[i] != yc[i]));
	}

	// export message
	// sing is (mlen, M, e, sign_i)
	// M includes its length, i.e., mlen
	*(unsigned long long*)sm = mlen;
	memcpy(sm+sizeof(unsigned long long), m, mlen);

	memcpy(sm+sizeof(unsigned long long)+mlen, sign->elem, sign->alloc_size);
	*(unsigned long long*)(sm + sizeof(unsigned long long) + mlen + ERRORSIZEBYTES) 
		= sign_i;

	*smlen = sizeof(unsigned long long) + mlen + ERRORSIZEBYTES + sizeof(unsigned long long);
	
	deleteMatrix(Sinv);
	deleteMatrix(synd_mtx);	deleteMatrix(scrambled_synd_mtx);
	deleteMatrix(error); deleteMatrix(error_p);
	free(yr); free(yc);
	// free(ctx);
	return 0;	
}
