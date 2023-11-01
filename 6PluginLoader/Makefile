all:	libfmadd.so libfmadd2.so main

run:	all
	@./main

memory:	main
	@valgrind ./main

main:	main.c
	@gcc -g -Wall -o main main.c -L/home/smarz1/courses/cosc360/stud/ -lcvector -ldl

libfmadd.so:	dingus.c 
	@gcc -Wall -O0 -g -shared -fPIC -o libfmadd.so dingus.c 

libfmadd2.so:	dingus2.c 
	@gcc -Wall -O0 -g -shared -fPIC -o libfmadd2.so dingus2.c 

clean:	
	rm -f main && rm -f libfmadd.so && rm -f libfmadd2.so
