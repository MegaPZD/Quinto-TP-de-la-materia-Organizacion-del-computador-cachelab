CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

OBJFILES=cachesim.o estructuras.o gestion_arg.o gestion_trazas.o operaciones.o

all: cachesim

cachesim: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c estructuras.h gestion_arg.h gestion_trazas.h operaciones.h
	$(CC) $(CFLAGS) -c $< -o $@

estructuras.o: estructuras.c estructuras.h
	$(CC) $(CFLAGS) -c $< -o $@

gestion_arg.o: gestion_arg.c gestion_arg.h
	$(CC) $(CFLAGS) -c $< -o $@

gestion_trazas.o: gestion_trazas.c gestion_trazas.h
	$(CC) $(CFLAGS) -c $< -o $@

operaciones.o: operaciones.c operaciones.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f cachesim

	

