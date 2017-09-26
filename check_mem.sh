#! /bin/bash
valgrind --leak-check=full --log-file=mem.log ./evp
