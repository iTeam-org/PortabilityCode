UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
   CC=clang
else
   CC=gcc
endif
CFLAGS=-I.
DEPS=portability.h
OBJ=test.o portability.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

libportab_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f libportab_test
	rm -Rf html
	rm -f *.zip

docu:
	doxygen ./doc/Doxyfile
