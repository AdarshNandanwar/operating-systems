executableName = driverModule

$(executableName) : $(executableName).c bubble_sort.o selection_sort.o insertion_sort.o merge_sort.o quick_sort.o
	gcc $(executableName).c bubble_sort.o selection_sort.o insertion_sort.o merge_sort.o quick_sort.o -o $(executableName)

bubble_sort.o : bubble_sort.c
	gcc -c bubble_sort.c -o bubble_sort.o

selection_sort.o : selection_sort.c
	gcc -c selection_sort.c -o selection_sort.o

insertion_sort.o : insertion_sort.c
	gcc -c insertion_sort.c -o insertion_sort.o

merge_sort.o : merge_sort.c
	gcc -c merge_sort.c -o merge_sort.o

quick_sort.o : quick_sort.c
	gcc -c quick_sort.c -o quick_sort.o

clean : 
	rm -f *.o