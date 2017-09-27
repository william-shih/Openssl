#ifndef ENC_KB_H
#define ENC_KB_H

#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors(){
	perror("Error");
 // Do Nothing
}

void load_pub_key(const char*, EVP_PKEY**); //default input pem key
void load_bio_der_pub_key(const char*, EVP_PKEY**); // input der key
void load_der_pub_key(const char*, EVP_PKEY**); // input file key
int envelope_seal(EVP_PKEY**, unsigned char*, int,unsigned char**, int*, unsigned char*, unsigned char*);
int Encrypt(unsigned char*, const int, unsigned char**);

#endif
