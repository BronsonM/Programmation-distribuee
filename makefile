all:projet1

projet1: main.o
	gcc -I/usr/include/python2.7 main.c CommunicationWebServer.c msh.c uart.c -L/usr/lib/python2.7/config/ -o main -lpython2.7 -ldl -lm -lutil -pthread
