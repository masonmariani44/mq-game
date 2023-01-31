game: game.c mq.o
	gcc -Wall -Werror -std=c11 -o game game.c mq.o -lm
mq.o: mq.c mq.h
	gcc -Wall -Werror -std=c11 -c mq.c
clean:
	rm -rf game mq.o
