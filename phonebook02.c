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
		// �ݺ��۾��� ���� ��� ���ѷ����� ����ϴ� ��찡 ����.
		// ���ϴ� ������ break;�� �ɾ��־� ������ �����Ų��. 
		
		printf("$ ");
		scanf("%s", command);
		// ����ڿ��� ���� �Է��� command�� ����
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
	scanf("%s", buf1); // �̸� 
	scanf("%s", buf2); // ��ȭ��ȣ 
	
	int i = n-1;
	while (i>=0 && strcmp(names[i], buf1) > 0){
	// strcmp�� ������ ������ ������� �ռ����� �񱳵� ���� 
	// ũ�ٴ� ���� ������ ���Ŀ��� ���ʿ� �ִٴ� �ǹ�
	// names[i]	> buf1 �� ��� �����  ��ȯ
	// names[i] < buf1 �� ��� ������  ��ȯ 
		names[i+1] = names[i];
		numbers[i+1] = numbers[i];
		i--; // i�� �ռ��� ���� üũ 
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
	scanf("%s", buf); //������ �̸� �Է¹��� 
	 
	int index = search(buf);
	// �Է��� ���ڿ��� �����ϸ� ��ġ(�ε���)�� ����
	// �������� ������ -1 ���� 
	 
	if (index == -1){
	 	printf("No person named '%s' exists.\n", buf);
	 	return;
	}
	
	int j = index; // �����Ǿ� ����ִ� �ε� �� 
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
	// �����͸� �о���� ���� "r" �б���� ����
	// fopen�� file pointer ���� �����ϸ� Ÿ���� FILE
	
	if (fp==NULL) {
		printf("Open failed.\n");
		return;
	// fopen �������� ��� ����ó�� 
	}
	
	while ((fscanf(fp, "%s", buf1) != EOF)) { // �̸��� �о� 
			//(����������, ���˽�Ʈ��, ����) 
			// fscanf�� ������ ���� �����ϸ� EOF(End of file)���� �����Ѵ�.
			// EOF�� �����ϱ� ������ �ݺ��ؼ� ������ �о�´�.
		fscanf(fp, "%s", buf2); //��ȭ��ȣ�� �о�� 
		names[n] = strdup(buf1);
		numbers[n] = strdup(buf2);
		n++;	
	}
	fclose(fp);
	// �۾��� �Ϸ��� ������ �ݵ�� fclose(����������) �Ѵ�. 
	
}

void save(){
	int i;
	char fileName[BUFFER_SIZE];
	char tmp[BUFFER_SIZE];
	
	scanf("%s", tmp); // which is "as", discarded
	scanf("%s", fileName); //������ ���� �̸�
	
	FILE * fp = fopen(fileName, "w"); 
	// ������ �����ϴ� ���̹Ƿ� wirte(w) ���
	// "w"���� ������ ���� ��� �����ϰ� ������� ������Ʈ�Ѵ�. 
	if (fp==NULL){
		printf("Open failed.\n");
		return;
	} 
	
	for(i=0; i<n; i++){
		fprintf(fp, "%s %s\n", names[i], numbers[i]);
		// fscanf�� ����ϴ�.
		// (����������, ���˽�Ʈ��, ����Ұ�)
	}
	
	fclose(fp);
	// �۾����� �� ������ �ݴ´�.
	// �ݾ����� ������ �����Ǿ� ���ɿ� ����
	 
}

