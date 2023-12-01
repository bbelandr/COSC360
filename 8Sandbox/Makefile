all: exe/sandbox samples

exe/sandbox:	*.c
	@gcc -Wall -g -o exe/sandbox *.c

samples:	TestFiles/sampleProgram.c
	@gcc -o TestFiles/sampleProgram TestFiles/sampleProgram.c

run:	all
	@./exe/sandbox

memory:	exe/sandbox
	@valgrind --leak-check=full ./exe/sandbox

clean:
	@rm -f exe/sandbox