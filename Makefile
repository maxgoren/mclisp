mgclisp:
	gcc -g eval.c list.c parse.c primitives.c repl.c specialform.c value.c -o lisp
