out : main.o tech.o list.o edit.o print.o parse.o io.o
	gcc main.o tech.o list.o edit.o print.o parse.o io.o -o out

main.o : main.c
	gcc -c main.c -g -Wall -ansi -pedantic -std=c90

tech.o : tech.c
	gcc -c tech.c -g -Wall -ansi -pedantic -std=c90

list.o : list.c
	gcc -c list.c -g -Wall -ansi -pedantic -std=c90

edit.o : edit.c
	gcc -c edit.c -g -Wall -ansi -pedantic -std=c90

print.o : print.c
	gcc -c print.c -g -Wall -ansi -pedantic -std=c90

parse.o : parse.c
	gcc -c parse.c -g -Wall -ansi -pedantic -std=c90

io.o : io.c
	gcc -c io.c -g -Wall -ansi -pedantic -std=c90
