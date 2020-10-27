linsearchtest.out: main.c lin_search.o
	gcc main.c lin_search.o -o linsearchtest.out

binsearchtest.out: main.c bin_search.o
	gcc main.c bin_search.o -o binsearchtest.out

bin_search.o: bin_search.c search.h
	gcc -c bin_search.c

lin_search.o: lin_search.c search.h
	gcc -c lin_search.c

clean:
	rm -f *.o *.out