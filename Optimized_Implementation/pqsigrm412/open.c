#include "api.h"
#include "common.h"

void import_signed_msg(matrix *errorMtx, unsigned long long *sign_i, const unsigned char *sm){
	importMatrix(errorMtx, sm);
	*sign_i = *((unsigned long long*)(sm+ERRORSIZEBYTES));
}

void build_public_mtx(matrix* H_pub, matrix* H_info, uint16_t *H_lead){
	size_t row, col, lidx = 0, infoidx = 0;

	for( col=0; (col < CODE_N) && (lidx < CODE_N - CODE_K); ++col){
		while(col < H_lead[lidx]){
			for(row = 0; row < CODE_N-CODE_K; ++row){
				setElement(H_pub, row, col, getElement(H_info, row, infoidx));
			}
			col ++; infoidx++;
		}
		setElement(H_pub, lidx, H_lead[lidx], 1);
		lidx++;

	}

	while(col < CODE_N){
			for(row = 0; row < CODE_N-CODE_K; ++row){
				setElement(H_pub, row, col, getElement(H_info, row, infoidx));
			}
			col ++; infoidx++;
		}
}

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk){
	matrix *errorMtx = newMatrix(1, CODE_N);

	matrix *H_pub = newMatrix(CODE_N-CODE_K, CODE_N);
	matrix *H_info = newMatrix(CODE_N-CODE_K, CODE_K);
	uint16_t *H_lead= (uint16_t*)(pk+PUBLIC_H_INFO_BYTES);

	matrix *syndrome_by_hash = newMatrix(1, CODE_N - CODE_K);
	matrix *syndrome_by_e	 = newMatrix(1, CODE_N - CODE_K);


	unsigned long long sign_i;
	unsigned long long mlen_rx;
	unsigned char* m_rx;
	
	int i;

	memcpy(&mlen_rx, sm, sizeof(unsigned long long));
	m_rx = (unsigned char*)malloc(mlen_rx);
	memcpy(m_rx, sm + sizeof(unsigned long long), mlen_rx);

	import_signed_msg(errorMtx, &sign_i, sm + sizeof(unsigned long long) + mlen_rx);

	if(hammingWgt(errorMtx) > WEIGHT_PUB) {
		return VERIF_REJECT;
	}

	unsigned char syndrome[SYNDROMESIZEBYTES]; 
	hashMsg(syndrome, m_rx, mlen_rx, sign_i);
	importMatrix(syndrome_by_hash, syndrome);

	//import public key
	importMatrix(H_info, pk);
	build_public_mtx(H_pub, H_info, H_lead);

	vector_mtx_product(syndrome_by_e, H_pub, errorMtx);

	for(i=0; i<CODE_N-CODE_K; ++i)
		if(getElement(syndrome_by_hash, 0, i) != getElement(syndrome_by_e, 0, i))
			return VERIF_REJECT;

	memcpy(m, m_rx, mlen_rx);
	*mlen = mlen_rx;

	deleteMatrix(errorMtx);
	deleteMatrix(H_pub);
	deleteMatrix(H_info);

	deleteMatrix(syndrome_by_hash);
	deleteMatrix(syndrome_by_e);
	free(m_rx);

	return 0;
}
