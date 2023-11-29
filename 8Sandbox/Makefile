all: exe/sandbox

exe/sandbox:	sandbox.c
	@gcc -Wall -g -o exe/sandbox sandbox.c

run:	exe/sandbox
	@./exe/sandbox

clean:
	@rm -f exe/sandbox