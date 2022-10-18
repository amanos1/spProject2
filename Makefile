CC     = gcc
CFLAGS = -g -Wall -Wvla -fsanitize=address

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM

all: tree.c ls.c find.c
	$(CC) $(CFLAGS) -o tree tree.c
	$(CC) $(CFLAGS) -o ls ls.c
	$(CC) $(CFLAGS) -o find find.c

