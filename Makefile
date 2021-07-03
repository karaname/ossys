CFLAGS = -g3 -O0 -Wall -Wextra
CC = gcc

sysinfo: sysinfo.o
	$(CC) $(CFLAGS) -o sysinfo sysinfo.o

sysinfo.o: sysinfo.c
	$(CC) $(CFLAGS) -c sysinfo.c -o sysinfo.o
