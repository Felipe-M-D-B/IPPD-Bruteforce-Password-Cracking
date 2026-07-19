FLAGS=-O3 -lssl -lcrypto

CC=gcc

RM=rm -f

EXEC=password_bf

INPUT=password.in

all: $(EXEC)

$(EXEC):
	$(CC) $(FLAGS) password_bf.c -c -o password_bf.o
	$(CC) password_bf.o -o $(EXEC) $(FLAGS)

run:
	./$(EXEC)

runtime:
	time -p ./$(EXEC) < $(INPUT)

clean:
	$(RM) password_bf.o $(EXEC)
