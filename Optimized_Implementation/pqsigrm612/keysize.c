#include <stdio.h>
#include "api.h"

int main(int argc, char const *argv[])
{
	printf("%d\n", CRYPTO_PUBLICKEYBYTES);
	printf("%d\n", CRYPTO_SECRETKEYBYTES);
	return 0;
}