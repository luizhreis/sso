compile:
	gcc -g -Wall -o sso sso.c

run:
	./sso -t 4 -l 100 -i process.txt -o log.log

debug:
	valgrind ./sso -t 4 -l 100 -i process.txt -o log.log
