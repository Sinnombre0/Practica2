# Configuracion para compilar
CC     = gcc
CFLAGS = -Wall -Wextra -g

all: emisor receptor

emisor: src/emisor.c
	$(CC) $(CFLAGS) -o emisor src/emisor.c

receptor: src/receptor.c
	$(CC) $(CFLAGS) -o receptor src/receptor.c

clean:
	rm -f emisor receptor