CC = gcc
LIBS = -lssl -lcrypto
CFLAGS = -g
OBJ = enc_kb.o dec_kb.o fio.o evp.o

%.o: %.c
#	@echo "input is $<, output is $@"
	$(CC) -c $< -o $@
evp: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

.PHONY: clean
clean:
	@echo "Clean Successfully"
	@rm -f $(OBJ)
	@rm -f evp ek iv *crypt.txt *.log
