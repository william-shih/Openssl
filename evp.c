#include <stdint.h>
#include "enc_kb.h"
#include "fio.h"

int main() {
  uint32_t len;
  unsigned char *enc_buf, *dec_buf, *enc, *dec;
  readfile("plain.txt", &enc_buf, &len);
  writefile("encrypt.txt", enc, Encrypt(enc_buf, len, &enc));
  readfile("encrypt.txt", &dec_buf, &len);
  writefile("decrypt.txt", dec, Decrypt(dec_buf, len, &dec));
  free(enc_buf);
  free(dec_buf);
  free(enc);
  free(dec);
  return 0;
}
