#include <openssl/evp.h>
#include <openssl/err.h>
#include "fio.h"

#define AES_BLOCK 16
#define aes_padding(x) (x) + AES_BLOCK + (x)%AES_BLOCK

void load_pub_key(const char *path, EVP_PKEY **key) {
    FILE *rsa_key_file = fopen(path, "r");
    if (rsa_key_file) {
        if (!PEM_read_PUBKEY(rsa_key_file, key, NULL, NULL)) {
            fprintf(stderr, "Error loading Public Key File.\n");
        }
        fclose(rsa_key_file);
    }
}

int envelope_seal(EVP_PKEY **pub_key, unsigned char *plaintext, int plaintext_len,
	unsigned char **encrypted_key, int *encrypted_key_len, unsigned char *iv,
	unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;

	int ciphertext_len;

	int len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the envelope seal operation. This operation generates
	 * a key for the provided cipher, and then encrypts that key a number
	 * of times (one for each public key provided in the pub_key array). In
	 * this example the array size is just one. This operation also
	 * generates an IV and places it in iv. */
	if(1 != EVP_SealInit(ctx, EVP_aes_256_cbc(), encrypted_key,
		encrypted_key_len, iv, pub_key, 1))
		handleErrors();

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_SealUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_SealUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

	/* Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_SealFinal(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int Encrypt(unsigned char* pxt, const int len, unsigned char** enc){
  int enc_len = 0;
  EVP_PKEY* pubkey = NULL;
  load_pub_key("public.key", &pubkey);

  int ivSize = EVP_MAX_IV_LENGTH, ekSize = EVP_PKEY_size(pubkey);
  unsigned char *ek = malloc(ekSize), *iv = malloc(ivSize);
  *enc = calloc(aes_padding(len), sizeof(unsigned char));

  enc_len =  envelope_seal(&pubkey, pxt, len, &ek, &ekSize, iv, *enc);
  writefile("ek", ek, ekSize);
  writefile("iv", iv, ivSize);
  free(ek);
  free(iv);
  EVP_PKEY_free(pubkey);
  return enc_len;
}
