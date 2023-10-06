all:	main


run:	main
	@./exe/main

memory:	main
	@valgrind ./exe/main

main:	main.c
	@gcc -Wall -g -o exe/main main.c

clean:
	rm -f exe/main
