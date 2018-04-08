all: UnixLs.c
	gcc -c UnixLs.c
	gcc -o UnixLs UnixLs.o
clean:
	-rm -f *.o UnixLs
