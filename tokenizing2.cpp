#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char str[] = "   study hard,     or   sleep.    ";
	char delim[] = " ";
	char *token;
	
	token = strtok(str, delim);
	
	while (token != NULL){
		printf("next token is: %s:%d\n", token, strlen(token));
		token = strtok(NULL, delim);
	}
	
	return 0;
