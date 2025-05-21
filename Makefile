mgclisp:
	gcc -g eval.c mgclisp.c list.c parse.c primitives.c main.c specialform.c atom.c -o mgclisp
install:
	mv ./mgclisp /usr/local/bin/mgclisp