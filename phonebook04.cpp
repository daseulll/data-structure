#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACITY 100
#define BUFFER_LENGTH 100

typedef struct person {
	char *name;
	char *number;
	char *email;
	char *group; 
} Person;
// typedef 으로 Person으로 renameing 

Person directory[CAPACITY];

int n=0;

int read_line( FILE * fp, char str[], int n );
int compose_name(char str[], int limit);
void add(char *name, char *number, char *email, char *group);
void find(char * name);
int search(char *name);
void status();
void remove1(char * name);
void load(char *fileName);
void handle_add(char * name);
void save(char * fileName);

int read_line(FILE * fp, char str[], int n) {
	// 키보드와 파일에서도 line을 읽어옴
	// 매개변수 추가 
	// n은 str[]의 크기  
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n' && ch != EOF )
		if (i<n)
			str[i++] = ch;
	
	str[i] = '\0';
	return i;
}


int main(){
	char command_line[BUFFER_LENGTH];
	char *command, *argument;
	char name_str[BUFFER_LENGTH];
	
	while (1) {
		printf("$ ");
		if (read_line(stdin, command_line, BUFFER_LENGTH)<=0)
			continue;
			
		command = strtok(command_line, " ");
		
		if (strcmp(command, "read") == 0) {
			argument = strtok(NULL, " ");
			if (argument == NULL) {
				printf("Invaild arguments.\n");
				continue;
			}
			load(argument);
		}
		
		else if(strcmp(command, "add") == 0){
			if (compose_name(name_str, BUFFER_LENGTH) <= 0){
				// compose_name함수로 불필요한 공백을 제거 
				printf("Name required.\n");
				continue;
			}
			handle_add(name_str);
		}
		
		else if(strcmp(command, "find") == 0){
			if (compose_name(name_str, BUFFER_LENGTH) <= 0){
				// compose_name함수로 불필요한 공백을 제거 
				printf("Name required.\n");
				continue;
			}
			find(name_str);
		}
		else if (strcmp(command, "status") ==0){
		
			status();
		}
		else if (strcmp(command, "delete") ==0 ){
			if (compose_name(name_str, BUFFER_LENGTH) <=0 ){
				printf("Invaild arguments.\n");
				continue;
			}
			remove1(name_str);
		}
		else if (strcmp(command, "save") ==0){
			argument = strtok(NULL, " ");
			if (strcmp(argument, "as") != 0){
				printf("Invalid arguments.\n");
				continue;
			}
			argument = strtok(NULL, " ");
			if (argument == NULL) {
				printf("Invalid arguments.\n");
				continue;
			}
			save(argument);
		}
		else if (strcmp(command, "exit") ==0){
			break;
		}
	}
	return 0;
}

void load(char *fileName) {
	char buffer[BUFFER_LENGTH];
	char *name, *number, *email, *group;
	
	FILE *fp = fopen(fileName, "r");
	if (fp==NULL) {
		printf("Open failed.\n");
		return;
	}
	
	while (1) {
		if (read_line(fp, buffer, BUFFER_LENGTH) <= 0)
			break;
		name = strtok(buffer, "#");
		number = strtok(NULL, "#");
		email = strtok(NULL, "#");
		group = strtok(NULL, "#");
		add(name, number, email, group);
	}
	fclose(fp);
}


void add(char *name, char *number, char *email, char *group){
	int i=n-1;
	while(i >= 0 && strcmp(directory[i].name, name) > 0){
		directory[i+1] = directory[i];
		// struct 이므로 통째로 옮겨진다.  
		i--;
	}
	
	directory[i+1].name = strdup(name);
	directory[i+1].number = strdup(number);
	directory[i+1].email = strdup(email);
	directory[i+1].group = strdup(group);
	
	n++;
}

int compose_name(char str[], int limit){
	char * ptr;
	int length = 0;
	
	ptr = strtok(NULL, " "); // 이름의 첫번째 단어 
	if (ptr == NULL)
		return 0; // 사람 이름의 길이가 0 
	
	strcpy(str, ptr);
	length += strlen(ptr);
	
	while ((ptr = strtok(NULL, " ")) != NULL) {
		if (length + strlen(ptr)+1 < limit){
			// null char를 위해 +1
			// 이름이 너무 길어서 BUFFER_LENGTH를 넘길 경우의 예외처리 
			
			str[length++] = ' '; // 단어와 단어사이 공백을줌  
			str[length] = '\0';
			strcat(str, ptr);
			// '\0'으로 끝날때에만 정상적으로 작동 
			// 앞의 strtok으로 생긴 '\0'를 ' '공백으로 바꾸었으므로 필요하다.
			//strcat 으로 str의 \0 null문자 위치부터 ptr가 이어붙는다.�. 
			length += strlen(ptr);
		}
		return length;
	}
	
}

void handle_add(char *name) {
	char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
	char empty[] = " ";
	
	printf(" Phone: ");
	read_line(stdin, number, BUFFER_LENGTH);
	
	printf(" Email: ");
	read_line(stdin, email, BUFFER_LENGTH);
	
	printf(" Group: ");
	read_line(stdin, group, BUFFER_LENGTH);
	
	add(name, (char *)(strlen(number)>0 ? number : empty),
				(char *)(strlen(email)>0 ? email : empty),
				(char*)(strlen(group)>0 ? group : empty));
	// 조건 ? A : B 조린퓽� 만족되면 A를 택하고, 아닐경우 B를 택함.
	 	
}


void save(char *fileName) {
	int i;
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL){
		printf("Open failed.\n");
		return;
	}
	
	for (i=0; i<n; i++){
		fprintf(fp, "%s#", directory[i].name);
		fprintf(fp, "%s#", directory[i].number);
		fprintf(fp, "%s#", directory[i].email);
		fprintf(fp, "%s#", directory[i].group);
		// 존재하지 않는 항목은 공백으로 저장했으므로
		//항목이 존재하느냐 여부를 따지지 않아도 된다. 
	}
	fclose(fp);
}	

int search(char *name) {
	int i;
	for (i=0; i<n; i++){
		if (strcmp(name, directory[i].name) ==0 ){
			return i;
		}
	}
	return -1;
}

void print_person(Person p){
	printf("%s:\n", p.name);
	printf("  Phone: %s\n", p.number);
	printf("  Email: %s\n", p.email);
	printf("  Group: %s\n", p.group);
}

void remove1(char *name) {
	int i = search(name);
	if (i == -1) {
		printf("No person named '%s' exists.\n", name);
		return;
	}
	
	int j = i;
	for ( ; j < n-1; j++) {
		directory[j] = directory[j+1];
		// 구조체 변수 간의 치환연산이 지원되므로 멤버 항목들을
		// 따로따로 치환할 필요가 없다. 
	}
	n--;
	printf("'%s' was deleted successfully.\n", name); 
}

void status(){
	int i;
	for (i=0; i<n; i++)
		print_person(directory[i]);
	printf("Total %d persons.\n", n);
}

void find(char *name) {
	int index = search(name);
	if (index == -1){
		printf("No person named '%s' exists.\n", name);
	}else{
		print_person(directory[index]);
	}
}



