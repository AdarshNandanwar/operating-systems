bstrunner.out: bstdriver.c getMin.o getMax.o addNode.o searchValue.o
	gcc -o bstrunner.out bstdriver.c getMax.o getMin.o searchValue.o addNode.o

searchValue.o: searchValue.c bst.h
	gcc -c searchValue.c

addNode.o: addNode.c bst.h
	gcc -c addNode.c

getMax.o: getMax.c bst.h
	gcc -c getMax.c

getMin.o: getMin.c bst.h
	gcc -c getMin.c

clean:
	rm -f *.o *.out

