CMD= main
SRCS= main.c make_graph.c hash_table.c cal_best.c game.c file_io.c utility.c
OBJS= $(SRCS:.c=.o)

CFLAGS+=-g
CC := clang

build: $(CMD)

$(CMD): $(OBJS)
	$(CC) $(CFLAGS) -o $(CMD) $(OBJS)

main.o: main.c make_graph.o hash_table.o cal_best.o game.o
	$(CC) $(CFLAGS) -c main.c -o main.o

make_graph.o: make_graph.c make_graph.h game.o hash_table.o utility.o
	$(CC) $(CFLAGS) -c make_graph.c -o make_graph.o 

cal_best.o: cal_best.c cal_best.h hash_table.o utility.o 
	$(CC) $(CFLAGS) -c cal_best.c -o cal_best.o

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

