exeName = driver

$(exeName): $(exeName).c cricket.o
	gcc $(exeName).c cricket.o -o $(exeName)

cricket.o: cricket.c
	gcc -c cricket.c -o cricket.o

clean: 
	rm -f *.o

run: 
	./$(exeName)