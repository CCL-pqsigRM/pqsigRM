#include "nearest_vector.h"
#include <stdlib.h>
#include "rng.h"
float *temp; 
void recursive_decoding_mod(float* y, const  int r1, const int m1, const int f, const int l, uint16_t *perm) {
	int i;
	if (r1 == 0) {
		//Calculate Euclidean distance
		int a1 = 0,a2 = 0;
	
		for ( i = f; i < l; i++) {
			// a1 += (y[i] - 1)*(y[i] - 1); a2 += (y[i] + 1)*(y[i] + 1);
			a1 += pow(y[i] - 1,2); a2 += pow(y[i] + 1,2);
		}
		if (a1 <= a2) 
			for ( i = f; i < l; i++) 
				y[i] = 1;
		else
			for ( i = f; i < l; i++) 
				y[i] = -1;
		return;
	}
	
	if (r1 == m1) {
		for ( i = f; i < l; i++) {
			if (y[i] >= 0)  y[i] = 1;
			else  y[i] = -1; 
		}
		return;
	}
	
	if(f == 1024 && l == 1536) // partial depermutation
		y_depermute(y, f, l, perm);
	
	
	for ( i = 0; i < (l - f) / 2; i++) {
		temp[f + i] = y[i + (l + f) / 2];
	}

	for ( i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}

	recursive_decoding_mod(y, r1 - 1, m1 - 1, (l + f) / 2, l, perm);

	for ( i = 0; i < (l - f) / 2; i++) {
		y[f + i] = (y[f + i] + y[i + (l + f) / 2] * temp[f + i]) / 2;
	}

	recursive_decoding_mod(y, r1, m1 - 1, f, (l + f) / 2, perm);

	for ( i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}
	
	if(f == 1024 && l == 1536) 
		y_permute(y, f, l, perm);
	
	return;
}

void init_decoding(int n){
	if(temp == 0) 
		temp = (float*)malloc(n * sizeof(float));

}
