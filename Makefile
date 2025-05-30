mgclisp:
	gcc -c src/eval.c 
	gcc -c src/mgclisp.c 
	gcc -c src/list.c 
	gcc -c src/parse.c 
	gcc -c src/primitives.c 
	gcc -c src/main.c 
	gcc -c src/specialform.c 
	gcc -c src/atom.c 
	gcc *.o -o mgclisp

install:
	cp ./src/stdlib.scm /usr/local/etc/
	mv ./mgclisp /usr/local/bin/mgclisp

clean:
	rm *.o