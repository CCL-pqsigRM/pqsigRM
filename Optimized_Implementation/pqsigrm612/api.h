//
//  api.h
//
#ifndef api_h
#define api_h

#define CRYPTO_PUBLICKEYBYTES 500780
#define CRYPTO_SECRETKEYBYTES 511020 

#define CRYPTO_BYTES 528

#define CRYPTO_ALGNAME "pqsigRM-6-12"

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
