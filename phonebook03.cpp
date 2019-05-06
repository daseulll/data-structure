#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 3 /*배열 재할당위해 일부러  작은 값으로*/
#define BUFFER_SIZE 50

char ** names;
char ** numbers;

int capacity = INIT_CAPACITY;
int n = 0;

char delim[] = " "; 

void init_directory();
void process_command();

void add(char *name, char *number);
void find(char * name);
int search(char *name);
void status();
void remove1(char * name);
void load(char *fileName);
void save(char * fileName);
void reallocate();

int main() {
	
	init_directory();
	process_command(); // 사용자 명령 별도 처리 
	
	return 0;
} 


void init_directory(){ // 동적 메모리 할 당 
	names = (char **)malloc(INIT_CAPACITY * sizeof(char *));
	numbers = (char **)malloc(INIT_CAPACITY * sizeof(char *));	
}
	
	
int read_line(char str[], int limit){
	// 엔터키를 칠 때 까지 한 줄을 입력받는다.
	// limit으로 사용자가 입력하는 str[] 길이를 제한한다. 
	int ch, i = 0;
	
	while ((ch = getchar()) != '\n')
	// getchar()은 문자 하나를 입력받지만, 정수값을 리턴한다. 
		if (i < limit-1)
			str[i++] = ch;
	
	str[i] = '\0';
	// limit - 1 까지만 ch를 저장하고 마지막엔 null character를 추가한다.
	
	return i; 
}

void process_command(){
	
	char command_line[BUFFER_SIZE];
	char *command, *argument1, *argument2;
	
	while(1) {
		printf("$ ");
		
		if(read_line(command_line, BUFFER_SIZE)<=0)
		// 아무것도 입력하지 않고 엔터친 경우 다음 $제공
			continue; 
		// 뒤의 코드 다 무시하고 처음으로 돌아감
		
		command = strtok(command_line, delim);
		// 첫번째 토큰은 명령어이다. (add, find 등) 
		if (command == NULL) continue;
		
		if (strcmp(command, "read")==0){
			argument1 = strtok(NULL, delim); // 파일명입력 
			if (argument1 == NULL){
				printf("File name required.\n");
				continue;
			}
			load(argument1);	 
		}
		
		else if (strcmp(command, "add") == 0){
			argument1 = strtok(NULL, delim);
			argument2 = strtok(NULL, delim);
			
			if (argument1 == NULL || argument2 == NULL){
				printf("Invalid arguments.\n");
				continue;
			}
			add(argument1, argument2);
			printf("%s was added successfully.\n", argument1);
		}
		else if (strcmp(command, "find") == 0){
			argument1 = strtok(NULL, delim);
			if (argument1 == NULL) {
				printf("Invalid arguments.\n");
			}
			find(argument1);
		}
		else if (strcmp(command, "status") == 0)
			status();	
		else if (strcmp(command, "delete") == 0){
			argument1 = strtok(NULL, delim);
			if (argument1 == NULL){
				printf("Invalid arguments.\n");
				continue;
			}
			remove1(argument1);
		}
		else if (strcmp(command, "save") ==0){
			argument1 = strtok(NULL, delim);
			argument2 = strtok(NULL, delim);
			if (argument1 == NULL || strcmp("as", argument1) != 0 
					|| argument2 == NULL) {
				printf("Invalid arguments.\n");
				continue;
			}
			save(argument2);
		}
		else if (strcmp(command, "exit") ==0)
			break;
	}
	
}

void add(char *name, char *number){
	if (n>=capacity)
		// n: 등록된 사람수, capacity : 배열의 크기 
		reallocate();
		// 배열이 꽉찬 경우 재할당한다. 
	
	int i = n-1;
	while (i>=0 && strcmp(names[i], name)>0){
		names[i+1] = names[i];
		numbers[i+1] = numbers[i];
		i--;
	}
	
	names[i+1] = strdup(name);
	numbers[i+1] = strdup(number);
	n++;	
}


void load(char *fileName){
	char buf1[BUFFER_SIZE];
	char buf2[BUFFER_SIZE];
	
	FILE * fp = fopen(fileName, "r");
	if (fp==NULL){
		printf("Open failed.\n");
		return;
	}
	while ((fscanf(fp, "%s", buf1) != EOF)){
		fscanf(fp, "%s", buf2);
		add(buf1, buf2);
	}
	fclose(fp);
}
	
void find(char * name){
	int index = search(name);
	if (index == -1)
		printf("No person named '%s' exists.\n", name);
	else
		printf("%s\n", numbers[index]);
}	

void status(){
	int i;
	
	for (i=0; i < n; i++){
		printf("%s %s\n", names[i], numbers[i]);
	}
	printf("Total %d persons.\n", n);
}
	
void remove1(char * name){
	int i = search(name);
	if (i == -1){
		printf("No person named '%s' exists.\n", name);
		return;
	}
	
	int j = i;
	for(; j<n-1; j++){
		names[j] = names[j+1];
		numbers[j] = numbers[j+1];
	}
	n--;
	printf("'%s' was deleted successfully.\n", name);
}

void save(char * fileName){
	int i;
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}
	
	for (i=0; i<n; i++){
		fprintf(fp, "%s %s\n", names[i], numbers[i]);
	}
	fclose(fp);
}

void reallocate(){
	// 더 큰 크기의 배열을 할당받는 것  
	
	int i;
	capacity *=2; // 크기를 2배 로 
	char **tmp1 = (char **)malloc(capacity*sizeof(char *));
	char **tmp2 = (char **)malloc(capacity*sizeof(char *));
	
	for (i=0; i<n; i++){
		tmp1[i] = names[i];
		tmp2[i] = numbers[i];
	}
	
	free(names);
	free(numbers);
	
	names = tmp1; // 주소를 새로 할당. 배열의이름은 포인터 변수이다. 
	numbers = tmp2;
}


