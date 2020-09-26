exeName = driver

$(exeName): $(exeName).c memTest.o initializeMemory.o addToMemory.o printMemoryState.o printScore.o
	gcc $(exeName).c initializeMemory.o memTest.o addToMemory.o printMemoryState.o printScore.o -o $(exeName)

memTest.o: memTest.c
	gcc -c memTest.c -o memTest.o

initializeMemory.o: initializeMemory.c
	gcc -c initializeMemory.c -o initializeMemory.o

addToMemory.o: addToMemory.c
	gcc -c addToMemory.c -o addToMemory.o

printMemoryState.o: printMemoryState.c
	gcc -c printMemoryState.c -o printMemoryState.o

printScore.o: printScore.c
	gcc -c printScore.c -o printScore.o

clean: 
	rm -f *.o

run: 
	./$(exeName) 4 input0.txt input1.txt output1.txt