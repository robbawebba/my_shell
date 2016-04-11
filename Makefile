CC=gcc

mysh: sh.o get_path.o parse_commandline.o pipe.o list.o sh.o alias.o command_list.o which.o where.o watchuser.o watchmail.o history.o signal_handlers.o pwd.o cd.o setenv.o kill.o warnload.o redirect.o main.c
	$(CC) -lpthread -lkstat -DHAVE_KSTAT -g main.c get_path.o parse_commandline.o pipe.o list.o sh.o alias.o command_list.o which.o where.o watchuser.o watchmail.o history.o signal_handlers.o pwd.o cd.o setenv.o kill.o warnload.o redirect.o -o mysh

get_path.o: get_path.c get_path.h
	$(CC) -g -c get_path.c

sh.o: sh.c sh.h
	$(CC) -g -c sh.c

parse_commandline.o: parse_commandline.c parse_commandline.h
	$(CC) -g -c parse_commandline.c

list.o: list.c list.h
	$(CC) -g -c list.c

alias.o: alias.c alias.h
	$(CC) -g -c alias.c

command_list.o: command_list.c command_list.h
	$(CC) -g -c command_list.c

which.o: which.c which.h
	$(CC) -g -c which.c

where.o: where.c where.h
	$(CC) -g -c where.c

watchuser.o: watchuser.c watchuser.h
	$(CC) -lpthread -g -c watchuser.c

watchmail.o: watchmail.c watchmail.h
	$(CC) -lpthread -g -c watchmail.c

warnload.o: warnload.c warnload.h
	$(CC) -lpthread -lkstat -DHAVE_KSTAT -g -c warnload.c


history.o: history.c history.h
	$(CC) -g -c history.c

signal_handlers.o: signal_handlers.c signal_handlers.h
	$(CC) -g -c signal_handlers.c

pwd.o: pwd.c pwd.h
	$(CC) -g -c pwd.c

cd.o: cd.c cd.h
	$(CC) -g -c cd.c

setenv.o: setenv.c setenv.h
	$(CC) -g -c setenv.c

kill.o: kill.c kill.h
	$(CC) -g -c kill.c

showload.o: showload.c showload.h
	$(CC) -lpthread -lkstat -DHAVE_KSTAT -g -c showload.c

redirect.o: redirect.c redirect.h
	$(CC) -g -c redirect.c

pipe.o: pipe.c pipe.h
	$(CC) -g -c pipe.c

clean:
	rm -rf *.o mysh
