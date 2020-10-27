executableName = file

$(executableName): file.c dep.o
	gcc file.c dep.o -o $(executableName)

dep.o: dep.c
	gcc -c dep.c -o dep.o

clean:
	rm -f *.o $(executableName)

run:
	./$(executableName)