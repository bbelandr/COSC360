all:	exe/main

exe/main:	src/*.c
	@gcc -g -Wall -o exe/main -I ./include src/*.c

run:	exe/main
	@./exe/main

memory:	exe/main
	@valgrind ./exe/main

clean:
	rm -f exe/main