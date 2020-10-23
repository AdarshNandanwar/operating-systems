#define READ_END 0

char str[1000];
read(pipe_end[READ_END], str, 1000);
printf("%s", str);