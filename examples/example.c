#include "../argo.h"

int main(int argc, char **argv) {
	
	int flag, integer;
	char character;
	double floating;
	char *string = NULL;
	
	//Define a list of arguments to be parsed
	argo_arg list[] = {
		ARGO_LIST_GROUP("Usage: -a <value>, --argument[=] <value>"),
		
		//A simple help (just --help, no -h) argument. It calls `argo_print_help` then exits with error level 0
		//If you want to customize the help message, view the macro's definition for help
		//Arguments of type ARGO_HELP will always exit the program, if you don't want that then make your help
		//   argument a flag and call the `argo_print_help` function manually
		ARGO_LIST_HELP(),
		
		//Groups are used specifically for help formating the help print function
		//If you don't plan on using my help print function then just ignore groups
		ARGO_LIST_GROUP("Arguments and whatnot"),
		
		//Flags are essentially booleans, the parsing will always set them to either 0 or 1
		ARGO_LIST_FLAG('f', "flag", &flag, "flag type argument"),
		
		//The pointer given is not checked to be valid, do not put NULL unless the character is 0 and the full name is NULL as well
		ARGO_LIST_INT('i', "integer", &integer, "integer type argument"),
		ARGO_LIST_CHAR('c', "char", &character, "char type argument"),
		ARGO_LIST_DOUBLE('d', "double", &floating, "double type argument"),
		
		//String arguments will point directly to their argv source, so be careful when editing them!
		ARGO_LIST_STRING('s', "string", &string, "string type argument"),
		
		//This is very important! Think of it as a null terminator
		ARGO_LIST_END()
	};
	
	//This function will allocate an array of strings (pointing to the strings in argv, NOT copies)
	//If you dont want to allocate anything, pass NULL to the function instead of a valid pointer
	//for example `argo_parse(NULL, argc, argv, list)` will always return 0 and never allocate anything
	char **leftover;
	
	//If the argument list is set up properly, this function should never fail (should... if you prove me wrong please let me know!)
	//It will return -1 and set leftover to NULL if the malloc fails, but all variables parsed as normal
	int leftoverCount = argo_parse(&leftover, argc, argv, list);
	
	
	printf("flag: %i, char: %c, integer: %i, double: %f, string: %s\n", flag, character, integer, floating, string);
	
	if (leftoverCount) {
		printf("Leftover vars: ");
		printf("%s", leftover[0]);
		for (int i = 1; i < leftoverCount; i++) {
			printf(", %s", leftover[i]);
		}
	}
	
	//Be sure to free the array of leftover arguments!
	free(leftover);
	
	return 0;
}