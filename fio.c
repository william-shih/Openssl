#include <stdlib.h>
#include <stdio.h>

void readfile (const unsigned char *path, unsigned char **buf, int *len) {
  printf("Read file %s.\n", path);
  FILE *fp = fopen(path, "r");
  fseek(fp, 0L, SEEK_END);
  *len = ftell(fp);
  rewind(fp);

  *buf = malloc(*len);
  fread(*buf, 1, *len, fp);

  fclose(fp);
}

void writefile (const unsigned char *path, unsigned char *buf, const int len){
  printf("Write file %s.\n", path);
  FILE *fp = fopen(path, "w");
  fwrite(buf, 1, len, fp);
  fclose(fp);
}
