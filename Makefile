FLAGS=-O3 -lssl -lcrypto

OMPFLAGS=-fopenmp

CC=gcc

RM=rm -f

EXEC=password_bf

EXEC_PARALLEL=password_bf_parallel

INPUT=password.in

all: runtime_parallel

runtime:
	$(CC) $(FLAGS) password_bf.c -c -o password_bf.o
	$(CC) password_bf.o -o $(EXEC) $(FLAGS)
	 /usr/bin/time -f "Tempo de execucao (sequencial): %es" ./$(EXEC) < $(INPUT)

runtime_parallel:
	$(CC) $(OMPFLAGS) $(FLAGS) password_bf_parallel.c -c -o password_bf_parallel.o
	$(CC) password_bf_parallel.o -o $(EXEC_PARALLEL) $(OMPFLAGS) $(FLAGS)
	 /usr/bin/time -f "Tempo de execucao (paralelo): %es" ./$(EXEC_PARALLEL) < $(INPUT)

clean:
	$(RM) password_bf.o $(EXEC)
	$(RM) password_bf_parallel.o $(EXEC_PARALLEL)
