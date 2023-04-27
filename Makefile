EXEC = quiz
OBJS = main.o quiz.o
CCFLAGS = gcc -Wall -std=c99 --pedantic
GCACHE = ./GameCache

${EXEC}: ${OBJS}
	${CCFLAGS} -o ${EXEC} ${OBJS}
	make gcache

# if the game cache does not exist, create it
gcache: | ${GCACHE}
	mv ${OBJS} ${EXEC} ${GCACHE}

${GCACHE}:
	mkdir -p $@

main.o: main.c
	${CCFLAGS} -c main.c

quiz.o: quiz.c
	${CCFLAGS} -c quiz.c

run: ${EXEC}
	${GCACHE}/${EXEC}