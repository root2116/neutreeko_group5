
CMD= main
SRCS= main.c make_graph.c hash_table.c test_util.c cal_best.c queue.c
OBJS= $(SRCS:.c=.o)

CFLAGS+=-g
CC := clang

build: $(CMD)

$(CMD): $(OBJS)
	$(CC) $(CFLAGS) -o $(CMD) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f "$(CMD)"
	rm -f *.o
	rm -f *.ilk
	rm -f *.pdb

