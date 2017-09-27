#ifndef DEC_KB_H
#define DEC_KB_H
#include "enc_kb.h"

void load_pri_key(const char*, EVP_PKEY**);
void load_bio_der_pri_key(const char*, EVP_PKEY**);
void load_der_pri_key(const char*, EVP_PKEY**);
int envelope_open(EVP_PKEY*, unsigned char*, int, unsigned char*, int, unsigned char*, unsigned char*);
int Decrypt(unsigned char*, const int, unsigned char**);

#endif
