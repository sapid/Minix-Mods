# Makefile heavily refactored from some of Wesley Mackey's code
# Author: Will Crawford <wacrawfo@ucsc.edu>
#	make			makes the shell
#	make clean		removes all object files
#	make spotless	as clean; also executables and generated files
#	make again		spotless, then all

CC = cc
NODEPS = clean spotless
EXE = shell
DEPSFILE = Makefile.deps
HSOURCES = 
CSOURCES = shell.c
LSOURCES = shell.l
YSOURCES = shell.y
CLGEN	 = shell.flex.c
HYGEN    = parser.h
CYGEN	 = shell.bison.c
CGENS    = ${CLGEN} ${CYGEN}
ALLGENS	 = ${HYGEN} ${CGENS}
ALLCSRC	 = ${CSOURCES} ${CLGEN} # ${CGENS}
COJBECTS = ${ALLCSRC:.c=.o}
CFLAGS	 = -g -Wall

all: ${EXE}

${DEPSFILE}:
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

${EXE}: ${COBJECTS}
	${CC} ${CFLAGS} -o $@ shell.c ${COBJECTS}
	
deps : ${ALLCSRC}
	${CC} -MM ${ALLCSRC} > Makefile.deps

${CLGEN} : ${LSOURCES}
	flex -o${CLGEN} ${LSOURCES} 2> ${LSOURCES:.l=.out}

${CYGEN} ${HYGEN} : ${YSOURCES}
	bison -dtv -o${CYGEN} --defines=${HYGEN} ${YSOURCES} \
	2> ${YSOURCES:.y=.out}

clean :
	rm -f ./*.o

spotless : clean
	rm -f ${EXE} ${ALLGENS} ${DEPSFILE} lex.backup
	rm -f ./*.out ./*.output

again : spotless
	${MAKE} all

ifeq "${filter ${NODEPS}, ${MAKECMDGOALS}}" ""
include ${DEPSFILE}
endif