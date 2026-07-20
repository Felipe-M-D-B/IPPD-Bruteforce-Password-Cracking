FLAGS=-O3 -lssl -lcrypto

OMPFLAGS=-fopenmp

CC=gcc

RM=rm -f

EXEC=password_bf

EXEC_PARALLEL=password_bf_parallel

INPUT=password.in

all: $(EXEC) $(EXEC_PARALLEL)

$(EXEC):
	$(CC) $(FLAGS) password_bf.c -c -o password_bf.o
	$(CC) password_bf.o -o $(EXEC) $(FLAGS)

$(EXEC_PARALLEL):
	$(CC) $(OMPFLAGS) $(FLAGS) password_bf_parallel.c -c -o password_bf_parallel.o
	$(CC) password_bf_parallel.o -o $(EXEC_PARALLEL) $(OMPFLAGS) $(FLAGS)

runtime: $(EXEC)
	 /usr/bin/time -f "Tempo de execucao (sequencial): %es" ./$(EXEC) < $(INPUT)

runtime_parallel: $(EXEC_PARALLEL)
	 /usr/bin/time -f "Tempo de execucao (paralelo): %es" ./$(EXEC_PARALLEL) < $(INPUT)

clean:
	$(RM) password_bf.o $(EXEC)
	$(RM) password_bf_parallel.o $(EXEC_PARALLEL)
