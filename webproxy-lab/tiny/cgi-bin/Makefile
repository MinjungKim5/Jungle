CC = gcc
CFLAGS = -O2 -Wall -I ..

all: adder hanoi

adder: adder.c
	$(CC) $(CFLAGS) -o adder adder.c

hanoi: hanoi.c
	$(CC) $(CFLAGS) -o hanoi hanoi.c

clean:
	rm -f adder *~
	rm -f hanoi *~
