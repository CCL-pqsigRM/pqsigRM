#include "api.h"
#include "nearest_vector.h"
#include "common.h"

#define SEEDEXPANDER_MAX_LEN 0xfffffffe // 2^32-1

void read_lead_diff(uint16_t *lead_diff){
	FILE *lead_diff_file = fopen("lead_diff.pqsigrm","rb");
	fread((unsigned char*)lead_diff,1, LEAD_DIFF_FILESIZE, lead_diff_file);
	fclose(lead_diff_file);
}

matrix* syndromeForMsg(matrix* scrambled_synd_mtx, matrix *Sinv, matrix *synd_mtx, const unsigned char *m, 
						unsigned long long mlen, unsigned long long sign_i)
{
	hashMsg(synd_mtx->elem, m, mlen, sign_i);

	vector_mtx_product(scrambled_synd_mtx, Sinv, synd_mtx);

	return scrambled_synd_mtx;
}

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
	const unsigned char *m, unsigned long long mlen,
	const unsigned char *sk){
	uint16_t lead_diff[CODE_N-CODE_K];
	read_lead_diff(lead_diff);

	// read secret key(bit stream) into appropriate type.
	matrix* Sinv = newMatrix(CODE_N-CODE_K, CODE_N-CODE_K);
	matrix* R = newMatrix(NUMOFPUNCTURE, CODE_N-NUMOFPUNCTURE);
	uint16_t* Qinv;

	//import secret keys
	importMatrix(Sinv, sk);
	importMatrix(R   , sk+SECRET_SINV_BYTES);
	Qinv = (uint16_t*)(sk+SECRET_SINV_BYTES+SECRET_R_BYTES);

	// Do signing:
	//		find e' such that wt(e') <= t
	//		iterate for SIGN_ITER_N times.
	// Signing time is proportional to Proportional 
	// To increase security, reduce t and increase N. 
	// This is security trade-off
	unsigned int i, j;
	
	unsigned long long sign_i;

	unsigned char sign[CODE_N];
	matrix *synd_mtx= newMatrix(1, CODE_N - CODE_K);
	matrix *scrambled_synd_mtx = newMatrix(1, CODE_K - CODE_K);

	float y[CODE_N];
	float not_decoded[CODE_N];
	unsigned char error[CODE_N];

	unsigned char seed[32];
	for(i =0; i<32; ++i)
		seed[i] = i;
	unsigned char div[8];
	for (i = 0; i<8; ++i)
		div[i] = i;

	AES_XOF_struct *ctx = (AES_XOF_struct*)malloc(sizeof(AES_XOF_struct));
	seedexpander_init(ctx, seed, div, SEEDEXPANDER_MAX_LEN);
	while(1){
		seedexpander(ctx, (unsigned char*)&sign_i, sizeof(unsigned long long));//random number
		// Find syndrome
		syndromeForMsg(scrambled_synd_mtx, Sinv, synd_mtx, m, mlen, sign_i);

		// decode and find e
		// In the recursive decoding procedure,
		// Y is 1 when the received codeword is 0, o.w, -1
		for(i=0; i<CODE_N; i++){
			not_decoded[i] = y[i] = 1;
		}
		for (i = 0; i < CODE_N - CODE_K; i++) {
			not_decoded[lead_diff[i]] = y[lead_diff[i]] 
				= (getElement(scrambled_synd_mtx, i, 0)==(unsigned char)0)? 1 : -1;
		}

		nearest_vector(y);

		// recover error for H_m
		for (i = 0; i < CODE_N; ++i) 
			error[i] = (y[i] != not_decoded[i]);

		// get e_p' using R
		unsigned char err;
		for (i = 0; i < NUMOFPUNCTURE; i++) {
			err = getElement(scrambled_synd_mtx, (CODE_N -CODE_K - NUMOFPUNCTURE) + i, 0);
			for (j = 0; j < (CODE_N - NUMOFPUNCTURE); j++) {
				err ^= (getElement(R, i, j) & error[j]);
			}
			error[(CODE_N - NUMOFPUNCTURE)+ i] = err;
		}

		// Check Hamming weight of e'
		if(hammingWgt(error, CODE_N) <= WEIGHT_PUB){
			break;
		}
	}

	// compute Qinv*e'
	for(i=0; i<CODE_N; i++){
		sign[Qinv[i]] = error[i];
	}

	// export message
	// sing is (mlen, M, e, sign_i)
	// M includes its length, i.e., mlen
	*(unsigned long long*)sm = mlen;
	memcpy(sm+sizeof(unsigned long long), m    , mlen);

	//export, write sign into bytes
	unsigned char byte;
	for(i=0; 8*i<CODE_N; ++i){
		byte=0;
		for(j=0; j<8; ++j)
			byte ^= (sign[8*i+j] << ((8-1)-j));
		*(sm + sizeof(unsigned long long) + mlen + i)= byte;
	}
	
	*(unsigned long long*)(sm + sizeof(unsigned long long) + mlen + ERRORSIZEBYTES) 
		= sign_i;

	*smlen = sizeof(unsigned long long) + mlen + ERRORSIZEBYTES + sizeof(unsigned long long);


	deleteMatrix(Sinv);
	deleteMatrix(R);
	deleteMatrix(synd_mtx);
	deleteMatrix(scrambled_synd_mtx);
	free(ctx);

	return 0;	
}
