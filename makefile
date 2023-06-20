CC=gcc
CFLAGS=-Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2 -g
LDFLAGS=-shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

all: libseq.so 
str.o: str.c str.h
	@$(CC) $(CFLAGS) -c str.c
seq.o: seq.c seq.h str.h
	@$(CC) $(CFLAGS) -c seq.c
seq_example.o: seq_example.c seq.h memory_tests.h
	@$(CC) $(CFLAGS) -c seq_example.c
memory_tests.o: memory_tests.h memory_tests.c 
	@$(CC) $(CFLAGS) -c memory_tests.c 
example: libseq.so seq_example.o 
	@$(CC) $(CFLAGS) -o example -g seq_example.o -L. -lseq
libseq.so: seq.o str.o memory_tests.o 
	@$(CC) $(CFLAGS) $(LDFLAGS) -o libseq.so $^
clean: 
	@rm -f *.o test libseq.so
