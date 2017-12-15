#include <stdio.h>
#include "matrix.h"

int main(){
	matrix *m1 = newMatrix(8,8);
	matrix *vec = newMatrix(1,8);
	matrix *res = newMatrix(1,8);

	unsigned int bit;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			scanf("%u", &bit);
			printf("%u ", bit);
			setElement(m1, i,j,bit);
		}putchar('\n');
	}

	for(int i=0; i<8; i++){
		scanf("%u", &bit);
		printf("%u", bit);
		setElement(vec, 0,i,bit);
	}

	printf("vec: %02x\n", vec->elem[0]);
	
	vector_mtx_product(res, m1, vec);

	printf("res: %02x\n", res->elem[0]);
	
	return 0;
}
