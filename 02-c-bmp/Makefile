CC = gcc

CFLAGS = -std=c17 -Iinclude -Wall -Werror -Wextra -pedantic 
SRC = src/main.c src/bmp.c
OBJ = obj/main.o obj/bmp.o

hw-01_bmp: $(OBJ)
	$(CC) $(CFLAGS) -o hw-01_bmp $(OBJ)

obj:
	mkdir -p obj

obj/main.o:  src/main.c | obj
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/bmp.o:  src/bmp.c | obj
	$(CC) $(CFLAGS) -c src/bmp.c -o obj/bmp.o

clean:
	rm -f $(OBJ) hw-01_bmp 
	rm -fr obj

.PHONY: clean



