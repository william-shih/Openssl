#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include "fio.h"

void load_pri_key(const char *path, EVP_PKEY **key){
    FILE *rsa_key_file = fopen(path, "r");
    if (rsa_key_file){
        *key = PEM_read_PrivateKey(rsa_key_file, NULL, NULL, NULL);
        if(!key){
            fprintf(stderr, "Error loading Private Key File.\n");
        }
    }
    fclose(rsa_key_file);
}

void load_der_pri_key(const char *path, EVP_PKEY **key){
    unsigned char *buf;
    int key_len = 0;

    readfile(path, &buf, &key_len);
    BIO *keyBio = BIO_new_mem_buf(buf, key_len);
    *key = d2i_PrivateKey_bio(keyBio, NULL);

    BIO_free_all(keyBio);
    free(buf);
}

int envelope_open(EVP_PKEY *priv_key, unsigned char *ciphertext, int ciphertext_len,
	unsigned char *encrypted_key, int encrypted_key_len, unsigned char *iv,
	unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;


	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the decryption operation. The asymmetric private key is
	 * provided and priv_key, whilst the encrypted session key is held in
	 * encrypted_key */
	if(1 != EVP_OpenInit(ctx, EVP_aes_256_cbc(), encrypted_key,
		encrypted_key_len, iv, priv_key))
		handleErrors();

	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_OpenUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_OpenUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	/* Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_OpenFinal(ctx, plaintext + len, &len)) handleErrors();
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

int Decrypt(unsigned char* ctx, const int len, unsigned char **dec){
  int dec_len = 0;
  EVP_PKEY* privkey = NULL;
  //load_pri_key("private.key", &privkey);
  load_der_pri_key("der_private.key", &privkey);
  int ekSize, ivSize;
  unsigned char* ek, *iv;

  readfile("ek", &ek, &ekSize);
  readfile("iv", &iv, &ivSize);
  *dec = calloc(len, sizeof(unsigned char));
  dec_len = envelope_open(privkey, ctx, len, ek, ekSize, iv, *dec);

  free(ek);
  free(iv);
  EVP_PKEY_free(privkey);
  return dec_len;
}
