UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
   CC=clang
else
   CC=gcc
endif
CFLAGS=-I.
DEPS=portability.h
OBJ=main.o portability.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

libportab_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm -vf *.o
	@rm -vf libportab_test

