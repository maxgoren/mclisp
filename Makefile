mgclisp:
	gcc -g eval.c mgclisp.c list.c parse.c primitives.c main.c specialform.c atom.c -o mgclisp
install:
	cp ./stdlib.scm /usr/local/etc/
	mv ./mgclisp /usr/local/bin/mgclisp