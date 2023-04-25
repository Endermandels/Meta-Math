EXEC = quiz
OBJS = main.o quiz.o
CCFLAGS = gcc -Wall -std=c99 --pedantic

${EXEC}: ${OBJS}
	${CCFLAGS} -o ${EXEC} ${OBJS}

main.o: main.c
	${CCFLAGS} -c main.c

quiz.o: quiz.c
	${CCFLAGS} -c quiz.c

run: ${EXEC}
	./${EXEC}