//
//  api.h
//
#ifndef api_h
#define api_h

#define CRYPTO_SECRETKEYBYTES 2129888
#define CRYPTO_PUBLICKEYBYTES 2105344

#define CRYPTO_BYTES 1040

#define CRYPTO_ALGNAME "pqsigRM-6-13"

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
