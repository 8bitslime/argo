# Argo
A simple command line argument parsing header file.
It is based on argparse, but made to be a header-only library.
The library was designed to be as simple to use as possible. The code isn't the nicest at the moment, but I plan to clean it up over time.

Arguments are specified in a number of ways resembling GNU's `getop`,  
	shorthand: `-f<value>`, `-f <value>`  
	full name: `--fullName=<value>`, `--fullName <value>`  

Types of arguments:

- Flag: takes int pointer, no input value, will always set the pointer to either 1 or 0, can specify multiple flags in shorthand i.e. `-abc` (sets flags a, b, and c)
- Int: takes int pointer, receives valid atoi strings, will not affect its pointer if the argument is not passed
- Char: takes char pointer, receives first character of inputted string, will not affect its pointer if the argument is not passed
- Double: takes double pointer, receives valid atof strings, will not affect its pointer if the argument is not passed
- String: takes `char*` pointer, receives strings, will not affect its pointer if the argument is not passed

## Usage
Include the header file `argo.h` then create an array of `argo_arg`s and finally call the function `argo_parse` and boom! You're done!

`argo_parse` is defined as: `int argo_parse(char ***leftover, int argc, char **argv, argo_arg *list)`

- **leftover**: pointer to a `char**`, will be set to a newly allocated array of strings containing all un-parsed arguments. You can set this to `NULL` if you do not want anything to be allocated.
- **argc**: `int`, argument from main function unaltered.
- **argv**: `char**`, argument from main function unaltered, will remain unaltered.
- **list**: `argo_arg*`, array of arguments to parse for, will remain unaltered.

return: `int`, size of the array of leftovers If it returns 0, leftovers will be set to NULL. If it returns -1 it means that the allocation has failed, but parsing was successful; do with that knowledge as you will.


I've added an example program showing the most basic usage of the library with descriptive comments in [`examples/example.c`](examples/example.c)
