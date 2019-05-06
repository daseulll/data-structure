#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char str[] = "now # is the time # to start preparing ### for the exam#";
	char delim[] = "#";
	char *token;
	
	token = strtok( str, delim );
	// (배열의이름(시작주소), delimeter) 
	// 리턴값은 첫번째 토큰의 시작주소를 리턴한다.
	// 여기서는 n의 주소를 갖는다.
	 
	while ( token != NULL ){
	// tokenizing이 끝나면 null을 리턴한다. 
		printf("netx token is: %s:%d\n", token, strlen(token));
		token = strtok( NULL, delim );
		// 두번째로 strtok을 호출할 때 첫매개변수는 NULL이 들어간다.
		// #로 자른 두번째 문자열의 첫번째 토큰의 시작주소를 리턴한다. 
		// 이에 대해 논란이 많다. (여기서 자세히 다루지는 않는다.) 
	}
	return 0;
}
