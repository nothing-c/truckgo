CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c99 -pedantic -g -fstack-protector

install: truckgo
	echo 'CC=gcc' > ~/.truckgo; echo 'DEBUG=-Wall -Wextra -Werror -std=c99 -pedantic -g -fstack-protector' >> ~/.truckgo; echo 'RELEASE=-Wall -std=c99 -O2' >> ~/.truckgo;

truckgo: main.o 
	$(CC) $(FLAGS) main.o -o truckgo

main.o: main.c truckgo.h
	$(CC) $(FLAGS) -c main.c
