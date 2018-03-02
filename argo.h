#ifndef _ARGO_INCLUDE_H
#define _ARGO_INCLUDE_H

// Simple command line argument parsing header file. Enjoy!

// (c) 2018 Zachary Wells
// This code is licensed under MIT license (see LICENSE for details)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef ARGO_MAX_LEFTOVERS
#define ARGO_MAX_LEFTOVERS 128
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum argo_type {
	ARGO_END = 0,
	ARGO_GROUP,
	ARGO_HELP,
	
	ARGO_FLAG,
	ARGO_INT,
	ARGO_CHAR,
	ARGO_DOUBLE,
	ARGO_STRING
} argo_type;

typedef struct argo_arg {
	char      character; // -f
	char      *fullName; // --fullName
	void      *ptr;      // pointer to where to store result (or function pointer)
	argo_type type;      // type of argument
	char      *message;  //message to print if '--help' is used
} argo_arg;

#define ARGO_LIST_HELP() {0, "help", NULL, ARGO_HELP, "print this message then exit"}
#define ARGO_LIST_GROUP(string) {0, NULL, NULL, ARGO_GROUP, string}
#define ARGO_LIST_FLAG(character, fullName, ptr, message)   {character, fullName, ptr, ARGO_FLAG,   message}
#define ARGO_LIST_INT(character, fullName, ptr, message)    {character, fullName, ptr, ARGO_INT,    message}
#define ARGO_LIST_CHAR(character, fullName, ptr, message)   {character, fullName, ptr, ARGO_CHAR,   message}
#define ARGO_LIST_DOUBLE(character, fullName, ptr, message) {character, fullName, ptr, ARGO_DOUBLE, message}
#define ARGO_LIST_STRING(character, fullName, ptr, message) {character, fullName, ptr, ARGO_STRING, message}
#define ARGO_LIST_END() {0, NULL, NULL, 0, NULL}

static void argo_print_help(argo_arg *list) {
	
	char *strings[ARGO_STRING + 1];
	strings[ARGO_HELP]   = "      ";
	strings[ARGO_FLAG]   = "      ";
	strings[ARGO_INT]    = "=<int>";
	strings[ARGO_CHAR]   = "=<char>";
	strings[ARGO_DOUBLE] = "=<double>";
	strings[ARGO_STRING] = "=<string>";
	
	int firstGroup = 1;
	for (int i = 0; list[i].type != ARGO_END; i++) {
		switch(list[i].type) {
		case ARGO_HELP:
		case ARGO_FLAG:
		case ARGO_INT:
		case ARGO_CHAR:
		case ARGO_DOUBLE:
		case ARGO_STRING:
			printf("  ");
			if (list[i].character) {
				printf("-%c%c ", list[i].character, list[i].fullName == NULL ? ' ' : ',');
			} else {
				printf("    ");
			}
			if (list[i].fullName) {
				printf("--%s%s", list[i].fullName, strings[list[i].type]);
				if (strlen(list[i].fullName) < 5) {
					printf("\t");
				} else if (strlen(list[i].fullName) < 8) {
					printf("    ");
				}
			} else {
				printf("\t\t\t");
			}
			if (list[i].message) {
				printf("\t%s", list[i].message);
			}
			printf("\n");
			continue;
		
		case ARGO_GROUP:
			if (firstGroup) {
				printf("%s\n", list[i].message);
				firstGroup = 0;
			} else {
				printf("\n%s\n", list[i].message);
			}
			continue;
		
		case ARGO_END:
		default:
			continue;
		}
	}
}

static inline void _argo_fulfil_arg(argo_arg *arg, char *value) {
	switch (arg->type) {
	case ARGO_FLAG:
		*((int*)arg->ptr) = 1;
	
	case ARGO_INT:
		*((int*)arg->ptr) = atoi(value);
		return;
	
	case ARGO_CHAR:
		*((char*)arg->ptr) = *value;
		return;
	
	case ARGO_DOUBLE:
		*((double*)arg->ptr) = atof(value);
		return;
	
	case ARGO_STRING:
		*((char**)arg->ptr) = value;
		return;
	
	case ARGO_END:
	default:
		return;
	}
}

static int argo_parse(char ***leftover, int argc, char **argv, argo_arg *list) {
	int leftoverCount = 0;
	int maxLeftovers = ARGO_MAX_LEFTOVERS;
	
	#if ARGO_MAX_LEFTOVERS == 0
		int leftovers[1];
	#else
		int leftovers[ARGO_MAX_LEFTOVERS];
	#endif
	
	if (leftover == NULL) {
		maxLeftovers = 0;
	}
	
	for (int j = 0; list[j].type != ARGO_END; j++) {
		if (list[j].type == ARGO_FLAG) {
			*((int*)list[j].ptr) = 0;
		}
	}
	
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			
			int matchFound = 0;
			
			for (int j = 0; list[j].type != ARGO_END; j++) {
				
				if (!list[j].character && !list[j].fullName) {
					continue;
				}
				
				int fullNameLength = 0;
				if (list[j].fullName) {
					fullNameLength = strlen(list[j].fullName);
				}
				
				if (fullNameLength && argv[i][1] == '-' && strncmp(list[j].fullName, argv[i] + 2, fullNameLength) == 0) {
					if (strlen(argv[i] + 2) > fullNameLength && argv[i][fullNameLength + 2] != '=') {
						continue;
					}
					
					if (list[j].type == ARGO_HELP) {
						argo_print_help(list);
						exit(0);
						return 0;
					} else if (list[j].type == ARGO_FLAG) {
						*((int*)list[j].ptr) = 1;
						matchFound = 1;
						break;
					}
					
					if (argv[i][fullNameLength + 2] == '=') {
						_argo_fulfil_arg(list + j, argv[i] + fullNameLength + 3);
						matchFound = 1;
						break;
					}
					
				} else if (list[j].character == argv[i][1]) {
					if (list[j].type == ARGO_HELP) {
						argo_print_help(list);
						exit(0);
						return 0;
					}
					
					int length = strlen(argv[i]);
					if (list[j].type == ARGO_FLAG) {
						*((int*)list[j].ptr) = 1;
						
						for (int k = 2; k < length; k++) {
							for (int jj = 0; list[jj].type != ARGO_END; jj++) {
								if (list[jj].type == ARGO_FLAG && list[jj].character == argv[i][k]) {
									*((int*)list[jj].ptr) = 1;
								}
							}
						}
						
						matchFound = 1;
						break;
					}
					if (length > 2) {
						_argo_fulfil_arg(list + j, argv[i] + 2);
						matchFound = 1;
						break;
					}
				} else {
					continue;
				}
				
				matchFound = 1;
				
				if (i + 1 < argc) {
					_argo_fulfil_arg(list + j, argv[++i]);
				} else {
					_argo_fulfil_arg(list + j, "");
				}
			}
			
			if (matchFound) {
				continue;
			}
		}
		
		if (leftoverCount < maxLeftovers) {
			leftovers[leftoverCount] = i;
			leftoverCount++;
		}
	}
	
	if (leftoverCount && maxLeftovers) {
		char **out = malloc(leftoverCount * sizeof(char*));
		
		if (out == NULL) {
			*leftover = NULL;
			return -1;
		}
		
		for (int i = 0; i < leftoverCount; i++) {
			out[i] = argv[leftovers[i]];
		}
		
		*leftover = out;
		return leftoverCount;
	} else {
		
		if (leftover != NULL) {
			*leftover = NULL;
		}
		return 0;
	}
}

#ifdef __cplusplus
}
#endif

#endif //ARGO_H