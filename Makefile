shell: myshell.o utils.o redirection.o commands.o signal.o
	cc myshell.o utils.o redirection.o commands.o signal.o -o myshell
mainshell: myshell.c
	cc -c myshell.c
utils: utils.c
	cc -c utils.c
redirection: redirection.c 
	cc -c redirection.c
commands: commands.c
	cc -c commands.c
signal: signal.c
	cc -c signal.c
