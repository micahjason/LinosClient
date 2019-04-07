.PHONY: all clean

all: linos_client

linos_client: Tmp/linos_client.o
	cc -Wall -g -o linos_client Tmp/linos_client.o

Tmp/linos_client.o: linos_client.c
	mkdir -p Tmp >/dev/null
	cc -c -I -Wall -g -o Tmp/linos_client.o linos_client.c

clean:
	rm -rf linos_client ./Tmp