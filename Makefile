OUTFILE =UnixLs
CROSS_COMPILE =
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -Werror -D _POSIX_C_SROUCE=200809L
all: app done
app:
	$(CC_C) $(CFLAGS) UnixLs.c -o $(OUTFILE)
done:
	@echo "Finished building application."
help:
	@echo " Build custom ls for unix"
clean:
	-rm -f OUTFILE 
