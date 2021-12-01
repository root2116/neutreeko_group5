CMD= main
SRCS= main.c perfect_analysis.c hash_table.c game.c file_io.c utility.c
OBJS= $(SRCS:.c=.o)

CFLAGS+=-g
CC := clang

build: $(CMD) preparation


$(CMD): $(OBJS)
	$(CC) $(CFLAGS) -o $(CMD) $(OBJS)

main.o: main.c hash_table.o game.o
	$(CC) $(CFLAGS) -c main.c -o main.o

preparation.o: preparation.c perfect_analysis.o 
	$(CC) $(CFLAGS) -c preparation.c -o preparation.o

preparation: preparation.o perfect_analysis.o hash_table.o game.o file_io.o utility.o
	$(CC) $(CFLAGS) -o preparation preparation.o perfect_analysis.o hash_table.o game.o file_io.o utility.o


perfect_analysis.o: perfect_analysis.c perfect_analysis.h game.o hash_table.o utility.o file_io.o
	$(CC) $(CFLAGS) -c perfect_analysis.c -o perfect_analysis.o 

game.o: game.c game.h hash_table.o utility.o
	$(CC) $(CFLAGS) -c game.c -o game.o

file_io.o: file_io.c file_io.h hash_table.o utility.o
	$(CC) $(CFLAGS) -c file_io.c -o file_io.o

utility.o: utility.c utility.h hash_table.o
	$(CC) $(CFLAGS) -c utility.c -o utility.o

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c -o hash_table.o

clean:
	rm -f "$(CMD)"
	rm -f *.o
	rm -f *.ilk
	rm -f *.pdb

