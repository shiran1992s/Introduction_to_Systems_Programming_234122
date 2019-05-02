CC = gcc
OBJS = series.o user.o mtmflix.o tests/mtmFlixTestEx.o
EXEC = mtmflix
DEBUG_FLAG = -DNDEBUG
CFLAGS = -std=c99 -Wall -pedantic-errors -Werror $(DEBUG_FLAG) mtm_ex3.o

$(EXEC) : $(OBJS) 
	$(CC) $(OBJS) mtm_ex3.o -o $@ -L. -lmtm
mtmFlixTestEx.o : tests/mtmFlixTestEx.c tests/test_utilities.h tests/../mtmflix.h tests/../mtm_ex3.h tests/../list.h
	$(CC) -c $(CFLAGS) tests/$*.c
mtmflix.o : mtmflix.c mtmflix.h mtm_ex3.h list.h user.h set.h series.h map.h
	$(CC) -c $(CFLAGS) $*.c
series.o : series.c series.h mtmflix.h mtm_ex3.h list.h set.h map.h user.h
	$(CC) -c $(CFLAGS) $*.c
user.o : user.c user.h mtmflix.h mtm_ex3.h list.h set.h series.h map.h
	$(CC) -c $(CFLAGS) $*.c

clean:
		rm -f $(OBJS) $(EXEC)