# Makefile heavily refactored from some of Wesley Mackey's code
# Author: Will Crawford <wacrawfo@ucsc.edu>
#	make			makes the shell
#	make clean		removes all object files
#	make spotless	as clean; also executables and generated files
#	make again		spotless, then all

CC = cc
NODEPS = clean spotless
EXE = shell
EXESRC	 = myshell.c
DEPSFILE = Makefile.deps
HSOURCES = 
CSOURCES =
LSOURCES = shell.l
YSOURCES = shell.y
CLGEN	 = shell.lex.c
HYGEN    = #shell.yacc.h
CYGEN	 = #shell.yacc.c
CGENS    = ${CLGEN} ${CYGEN}
ALLGENS	 = ${HYGEN} ${CGENS}
ALLCSRC	 = ${CSOURCES} ${CGENS}
COBJECTS = ${ALLCSRC:.c=.o}
CFLAGS	 = -g -Wall

all: ${EXE}

${DEPSFILE}:
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

${EXE}: ${COBJECTS}
	${CC} ${CFLAGS} -o $@ ${EXESRC} ${COBJECTS}
	
deps : ${ALLCSRC}
	${CC} -MM ${ALLCSRC} > Makefile.deps

${CLGEN} : ${LSOURCES}
	lex -o${CLGEN} ${LSOURCES} 2> ${LSOURCES:.l=.out}

${CYGEN} ${HYGEN} : ${YSOURCES}
	yacc -dtv -o${CYGEN} --defines=${HYGEN} ${YSOURCES} \
	2> ${YSOURCES:.y=.out}

clean :
	rm -f ./*.o

spotless : clean
	rm -f ${EXE} ${ALLGENS} ${DEPSFILE} lex.backup
	rm -f ./*.out ./*.output

again : spotless
	${MAKE} all
