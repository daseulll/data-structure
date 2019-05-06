#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAPACITY 100
#define BUFFER_LENGTH 100

typedef struct {
	char *name;
	char *number;
	char *email;
	char *group; 
} Person;
// typedef 으로 Person으로 renameing 

void init();
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
void reallocate();
void release_person(Person *p);

Person ** directory;
int capacity;
int n;


void init(){
	directory = (Person **)malloc(INIT_CAPACITY*sizeof(Person *));
	capacity = INIT_CAPACITY;
	n = 0;
}


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
	
	init();
	 
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
	char *token;
	
	FILE *fp = fopen(fileName, "r");
	if (fp==NULL) {
		printf("Open failed.\n");
		return;
	}
	
	while (1) {
		if (read_line(fp, buffer, BUFFER_LENGTH) <= 0)
			break;
		name = strtok(buffer, "#");
		token = strtok(NULL, "#");
		if (strcmp(token, " ") == 0)
			number = NULL;
		// 공백으로 나타난 존재하지 않는 값에 대해 NULL처리 
		else 
			number = strdup(token);
		token = strtok(NULL, "#");
		if (strcmp(token, " ") == 0)
			email = NULL;
		else
			email = strdup(token);
		token = strtok(NULL, "#");
		if (strcmp(token, " ") == 0)
			group = NULL;
		else
			group = strdup(token);
		add(strdup(name), number, email, group);
		// 복제된 것을 add 넘기므로, add에서는 strdup할 필요가 없다. 
	}
	fclose(fp);
}


void add(char *name, char *number, char *email, char *group){
	if (n>=capacity)
		reallocate(); 
	
	int i = n-1;
	while(i >= 0 && strcmp(directory[i]->name, name) > 0){
		directory[i+1] = directory[i];
		// 포인터변수이므로, 포인터 주소값만 옮겨진다. 
		i--;
	}
	
	directory[i+1] = (Person *)malloc(sizeof(Person));
	// 구조체가 한칸 씩 뒤로 옮겨졌으므로, 비어있는 칸에 새로 구조체를 동적할당
	// v4.0과 가장 큰 차이이다. 
	// malloc이 리턴해주는 새로운 주소값이 저장된다.
	 
	directory[i+1]->name = name;
	directory[i+1]->number = number;
	directory[i+1]->email = email;
	directory[i+1]->group = group;
	// 주소값을 저장하는 포인터변수이므로 -> 연산자를 사용한다. 
	
	n++;
}

void reallocate(){
	capacity *= 2;
	Person **tmp = (Person **)malloc(capacity * sizeof(Person *));
	for (int i=0; i<n; i++)
		tmp[i] = directory[i];
	free(directory);
	// free로 반환해주지 않으면 사용할수 없는 메모리, garbage로 남아 축적되면
	// heap용량이 부족해질 수 있어 성능 저하를 유발할 수 있다.
	// directory = tmp;이전에 free해주어야 이전 데이터가 날아간다. 
	// malloc으로 할당받은 메모리는 필요없어지면 반드시 free한다. 
	
	directory = tmp;
	// tmp배열의 주소를 directory로 바꾼다. 
	
	// reallocate를 할 때에도, 포인터 변수의 주소값을 복사하므로
	// 구조체 배열보다 훨씬 효율적이다. 
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
			//strcat 으로 str의 \0 null문자 위치부터 ptr가 이어붙는다.? 
			length += strlen(ptr);
		}
		return length;
	}
	
}

void handle_add(char *name) {
	char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
	
	printf(" Phone: ");
	read_line(stdin, number, BUFFER_LENGTH);
	
	printf(" Email: ");
	read_line(stdin, email, BUFFER_LENGTH);
	
	printf(" Group: ");
	read_line(stdin, group, BUFFER_LENGTH);
	
	add(strdup(name), strdup((char *)(strlen(number)>0 ? number : NULL)),
				strdup((char *)(strlen(email)>0 ? email : NULL)),
				strdup((char*)(strlen(group)>0 ? group : NULL)));
	// 조건 ? A : B 조린퓽?만족되면 A를 택하고, 아닐경우 B를 택함.
}


void save(char *fileName) {
	int i;
	FILE *fp = fopen(fileName, "w");
	if (fp == NULL){
		printf("Open failed.\n");
		return;
	}
	
	for (i=0; i<n; i++){
		fprintf(fp, "%s#", directory[i]->name);
		fprintf(fp, "%s#", directory[i]->number);
		fprintf(fp, "%s#", directory[i]->email);
		fprintf(fp, "%s#", directory[i]->group);
		// 존재하지 않는 항목은 공백으로 저장했으므로
		//항목이 존재하느냐 여부를 따지지 않아도 된다. 
	}
	fclose(fp);
}	

int search(char *name) {
	int i;
	for (i=0; i<n; i++){
		if (strcmp(name, directory[i]->name) == 0 ){
			return i;
		}
	}
	return -1;
}

void print_person(Person *p){
	printf("%s:\n", p->name);
	printf("  Phone: %s\n", p->number);
	printf("  Email: %s\n", p->email);
	printf("  Group: %s\n", p->group);
}

void remove1(char *name) {
	int i = search(name);
	if (i == -1) {
		printf("No person named '%s' exists.\n", name);
		return; 
	}
	
	Person *p = directory[i];
	// 포인터변수 p를 만들어 원하는 directory[i]의 값을 가져온다. 
	for (int j=i ; j < n-1; j++) {
		directory[j] = directory[j+1];
		// diretory에 저장된 포인터 주소값이 한칸 씩 앞으로 당겨짐 
	}
	n--;
	release_person(p); 
	// 모든 Person 자료는 strdup 내의 malloc으로 할당되었으므로 free해주는 함수를 수행한다.
	// 구조체에 연결돼있는 name, email 등 문자열들도 free시켜줘야 하므로 
	// free(p)는 모든 문자열을 free시킨후 마지막에 수행한다. 
	printf("'%s' was deleted successfully.\n", name); 
}

void release_person(Person *p){
	free(p->name);
	if (p->number != NULL) free(p->number);
	if (p->email != NULL) free(p->email);
	if (p->group != NULL) free(p->group);
	// NULL일 경우 free해줄 필요가 없으므로 조건문으로 제외한다. 
	free(p);
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



