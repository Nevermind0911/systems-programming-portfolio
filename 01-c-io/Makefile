CC = gcc 

CFLAGS = -std=c17 -Iinclude -Wall -Werror -Wextra -pedantic

SRC = src/main.c src/clist.c src/point_list.c
OBJ = obj/main.o obj/clist.o obj/point_list.o

lab-05_c-io: $(OBJ)
	$(CC) $(CFLAGS) -o lab-05_c-io $(OBJ)

obj:
	mkdir -p obj

obj/main.o: include/clist.h include/point_list.h src/main.c | obj
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/clist.o: include/clist.h src/clist.c | obj
	$(CC) $(CFLAGS) -c src/clist.c -o obj/clist.o

obj/point_list.o: include/point_list.h src/point_list.c | obj
	$(CC) $(CFLAGS) -c src/point_list.c -o obj/point_list.o

clean:
	rm -f $(OBJ) lab-05_c-io
	rm -fr obj

.PHONY: clean

