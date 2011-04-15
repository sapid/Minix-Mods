# Author: Will Crawford <wacrawfo@ucsc.edu>
#	make			makes the shell
#	make clean		removes all object files
#	make spotless	as clean; also executables and generated files
#	make again		spotless, then all

shell: dshell.c dlex.c
	gcc -g -Wall -o shell dshell.c dlex.c

dlex.c : dlex.l
	lex -l -odlex.c dlex.l 2> dlex.out
	sed '3s/.*/#define _POSIX_SOURCE/' dlex.c > dlex.c.tmp
	sed '7s/.*/#include<stdio.h>/' dlex.c.tmp > dlex.c
	rm dlex.c.tmp

clean :
	rm -f dlex.c dlex.out
	rm -rf dshell.dSYM shell.dSYM

spotless : clean
	rm -f shell

again : spotless
	${MAKE} shell
