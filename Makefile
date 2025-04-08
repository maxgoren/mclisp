mgclisp:
	gcc -g eval.c list.c parse.c primitives.c repl.c specialform.c atom.c -o lisp
install:
	mv ./lisp /usr/local/bin/mgclisp