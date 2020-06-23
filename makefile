HEADERS = lib/include/librele.h 

default: librele.o

librele.o: lib/src/librele.c  $(HEADERS)
	gcc -c lib/src/libcallidus.c  -o lib/src/libcallidus.o -Ilib/include
	gcc -c lib/src/librele.c lib/src/libcallidus.o  -o lib/librele.o -Ilib/include	

clean:
	-rm -f lib/librele.o
