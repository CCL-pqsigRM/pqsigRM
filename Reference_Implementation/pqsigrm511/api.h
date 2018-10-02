//
//  api.h
//
#ifndef api_h
#define api_h

#define CRYPTO_PUBLICKEYBYTES (1024*2048)/8 
#define CRYPTO_SECRETKEYBYTES (1024*1024+2048*16+512*16+512*16+1024*16)/8 

#define CRYPTO_BYTES (64+2048+64)/8 

#define CRYPTO_ALGNAME "pqsigRM-5-11"

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk);

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk);

#endif /* api_h */
