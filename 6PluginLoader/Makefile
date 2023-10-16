all:	libtest.so exe/main

run:	exe/main
	@./exe/main

memory:	exe/main
	@valgrind ./exe/main

exe/main:	main.c
	@gcc -Wall -g -o exe/main main.c



clean:	
	rm -f exe/main
