# File IO

## fgets()
- Haven't checked how this works
- Includes `\n` in the line read (except for the last line).
- If the last line is empty, it will read empty string and execute the loop.
```c
FILE * fd_in = fopen("input.txt", "r");
if(fd_in == NULL){
    printf("error opening \"input.txt\" file\n");
    return 0;
}

char line[1024];
while(fgets(line, sizeof(line), fd_in)){
    printf("[%ld] %s", sizeof(line), line); // size if always 1024
    // do stuff
}

fclose(fd_in);
```

## dup2()
- This method can't detect EOF. Only use if number of input lines are known.
- Dosen't include `\n` in the line read. Behaves exactly like `STDIN`.
```c
int fd_in = open("input.txt", O_RDONLY); 
if(fd_in<0) {
    perror("file cant be opened");
    return 0;
}
dup2(fd_in,0);  //(stdin :0)

int n;
char line[1024];
scanf("%d", &n);
while(n--){
    scanf("%s", &line);
    printf("[%ld] %s", sizeof(line), line); // size if always 1024
    // do stuff
}

close(fd_in);
```

# C syntax

## Scan complete line without space as delimiter
```c
scanf("%[^\n]%*c", line);
```
## To initalize a string
```c
#define N 1024
// DO:
char * str = (char *) malloc(N*sizeof(char));
strcat(str, "abcd");
// DON'T DO:
char * str = (char *) malloc(N*sizeof(char));
str = "abcd";
```