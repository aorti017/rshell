all:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -pedantic -ansi src/hw0.cpp -o bin/rshell
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
	g++ -Wall -Werror -ansi -pedantic -std=c++0x src/ls.cpp -o bin/ls

rshell:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/hw0.cpp -o bin/rshell

cp:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
ls:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic -std=c++11 src/ls.cpp -o bin/ls

