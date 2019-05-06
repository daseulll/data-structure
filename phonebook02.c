#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 100
#define BUFFER_SIZE 100

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

char * names[CAPACITY];
char * numbers[CAPACITY];

int n = 0;

void add();
void find();
void status();
void remove1();
void load();
void save();

int main(int argc, char *argv[]) {
	char command[BUFFER_SIZE];
	while(1) {
		// 반복작업을 원할 경우 무한루프를 사용하는 경우가 많다.
		// 원하는 지점에 break;를 걸어주어 루프를 종료시킨다. 
		
		printf("$ ");
		scanf("%s", command);
		// 사용자에게 받은 입력을 command에 저장
		if (strcmp(command, "read") == 0)
			load();
		else if (strcmp(command, "add") == 0)
			add();
		else if (strcmp(command, "find") == 0)
			find(); 
		else if (strcmp(command, "status") == 0)
			status();
		else if (strcmp(command, "delete") == 0)
			remove1();
		else if (strcmp(command, "save") == 0)
			save();
		else if (strcmp(command, "exit") == 0)
			break;
	
	}
	return 0;
}


void add(){
	char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
	scanf("%s", buf1); // 이름 
	scanf("%s", buf2); // 전화번호 
	
	int i = n-1;
	while (i>=0 && strcmp(names[i], buf1) > 0){
	// strcmp로 사전식 순서가 어느것이 앞서는지 비교도 가능 
	// 크다는 것은 사전식 정렬에서 뒤쪽에 있다는 의미
	// names[i]	> buf1 의 경우 양수를  반환
	// names[i] < buf1 의 경우 음수를  반환 
		names[i+1] = names[i];
		numbers[i+1] = numbers[i];
		i--; // i의 앞순서 문자 체크 
	}
	names[n] = strdup(buf1);
	numbers[n] = strdup(buf2);
	n++;
	
	printf("%s was added successfully.\n", buf1);
}

void find(){
	char buf[BUFFER_SIZE];
	scanf("%s", buf);
	
	int index = search(buf);
	
	if(index == -1){
		printf("No person named '%s' exists.", buf);
	}else{
		printf("%s\n", numbers[index]); 
	}

}

int search(char * name){
	int i;
	for (i=0; i<n; i++){
		if (strcmp(name, names[i]) == 0) {
			return i;  
		}
	}
	return -1;
}


void status(){
	int i;
	
	for (i=0; i < n; i++){
		printf("%s %s\n", names[i], numbers[i]);
	}
	printf("Total %d persons.\n", n);
	
}

void remove1(){
	char buf[BUFFER_SIZE];  
	scanf("%s", buf); //삭제할 이름 입력받음 
	 
	int index = search(buf);
	// 입력한 문자열이 존재하면 위치(인덱스)를 리턴
	// 존재하지 않으면 -1 리턴 
	 
	if (index == -1){
	 	printf("No person named '%s' exists.\n", buf);
	 	return;
	}
	
	int j = index; // 삭제되어 비어있는 인덱 스 
	for ( ; j < n-1; j++){
		names[j] = names[j+1];
		numbers[j] = numbers[j+1];
		
	}
	n--;
	printf("'%s' was deleted successfully.\n", buf);
	
}

void load(){
	char fileName[BUFFER_SIZE];
	char buf1[BUFFER_SIZE];
	char buf2[BUFFER_SIZE];
	
	scanf("%s", fileName);
	
	FILE * fp = fopen(fileName, "r");
	// 데이터를 읽어오기 위해 "r" 읽기모드로 열기
	// fopen은 file pointer 값을 리턴하며 타입은 FILE
	
	if (fp==NULL) {
		printf("Open failed.\n");
		return;
	// fopen 실패했을 경우 예외처리 
	}
	
	while ((fscanf(fp, "%s", buf1) != EOF)) { // 이름을 읽어 
			//(파일포인터, 포맷스트링, 변수) 
			// fscanf는 파일의 끝에 도달하면 EOF(End of file)값을 리턴한다.
			// EOF에 도달하기 전까지 반복해서 파일을 읽어온다.
		fscanf(fp, "%s", buf2); //전화번호부 읽어옴 
		names[n] = strdup(buf1);
		numbers[n] = strdup(buf2);
		n++;	
	}
	fclose(fp);
	// 작업을 완료한 파일은 반드시 fclose(파일포인터) 한다. 
	
}

void save(){
	int i;
	char fileName[BUFFER_SIZE];
	char tmp[BUFFER_SIZE];
	
	scanf("%s", tmp); // which is "as", discarded
	scanf("%s", fileName); //저장할 파일 이름
	
	FILE * fp = fopen(fileName, "w"); 
	// 파일을 수정하는 것이므로 wirte(w) 모드
	// "w"모드는 파일이 없을 경우 생성하고 있을경우 업데이트한다. 
	if (fp==NULL){
		printf("Open failed.\n");
		return;
	} 
	
	for(i=0; i<n; i++){
		fprintf(fp, "%s %s\n", names[i], numbers[i]);
		// fscanf과 비슷하다.
		// (파일포인터, 포맷스트링, 출력할값)
	}
	
	fclose(fp);
	// 작업종료 시 파일을 닫는다.
	// 닫아주지 않으면 축적되어 성능에 영향
	 
}

