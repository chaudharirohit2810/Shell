shell: myshell.o utils.o redirection.o
	cc myshell.o utils.o redirection.o -o myshell
	./myshell
mainshell: myshell.c
	cc -c myshell.c
utils: utils.c
	cc -c utils.c
redirection: redirection.c 
	cc -c redirection.o
