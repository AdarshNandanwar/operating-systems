# STEPS OF COMPILATION

NOTE: -o option is used to specify the name of the output file.

## PREPROCESSING
* The preprocessor obeys commands that begin with # (known as directives) by:
    * removing comments
    * expanding macros
    * expanding included files
* i file is the preprocessed c program
* It just copies the code from the included header files
* It also replaces the macros with the specified value in the code

```bash
$ gcc -E file.c -o file.i
```

## COMPILATION
* It takes the output of the preprocessor and generates assembly language, an intermediate human readable language, specific to the target processor.
* s file is the assembly code of the c program

```bash
$ gcc -S file.c -o file.s
```

## ASSEMBLY
* The assembler will convert the assembly code into pure binary code or machine code (zeros and ones). 
* This code is also known as object code.
* o file is the object file
* Object file is not human readable

```bash
$ gcc -c file.c -o file.o
```

## LINKING
* The linker merges all the object code from multiple modules into a single one. If we are using a function from libraries, linker will link our code with that library function code.
    * In static linking, the linker makes a copy of all used library functions to the executable file. 
    * In dynamic linking, the code is not copied, it is done by just placing the name of the library in the binary file.

```bash
$ gcc file.c -o file
```

