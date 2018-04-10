all: UnixLs.c
	gcc -c UnixLs.c
	gcc -g -o UnixLs UnixLs.o
clean:
	-rm -f *.o UnixLs
