HEADERS = lib/include/librele.h

default: librele.o

librele.o: lib/src/librele.c $(HEADERS)
	gcc -c lib/src/librele.c -o lib/librele.o -Ilib/include

clean:
	-rm -f lib/librele.o
