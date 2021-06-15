snake: snake.o main.o
	gcc -g snake.o main.o -lcurses -o snake
main.o: main.c snake.o
	gcc -c -g main.c -lcurses
snake.o: snake.c snake.h
	gcc -c -g snake.c -lcurses