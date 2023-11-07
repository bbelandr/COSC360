# If you're wondering about the include file, -I shows the GNU compiler where to look for .h files
# That way, you don't have to type the whole directory when you include a .h file
all:	../exe/main

../exe/main:	*.c
	@gcc -g -Wall -o ../exe/main -I ../include *.c -lpthread

run:	../exe/main
	@./../exe/main

memory:	../exe/main
	@valgrind ./../exe/main

clean:
	rm -f ../exe/main
