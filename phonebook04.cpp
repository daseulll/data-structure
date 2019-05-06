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
// typedef À¸·Î PersonÀ¸·Î renameing 

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
	// Å°º¸µå¿Í ÆÄÀÏ¿¡¼­µµ lineÀ» ÀÐ¾î¿È
	// ¸Å°³º¯¼ö Ãß°¡ 
	// nÀº str[]ÀÇ Å©±â  
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
				// compose_nameÇÔ¼ö·Î ºÒÇÊ¿äÇÑ °ø¹éÀ» Á¦°Å 
				printf("Name required.\n");
				continue;
			}
			handle_add(name_str);
		}
		
		else if(strcmp(command, "find") == 0){
			if (compose_name(name_str, BUFFER_LENGTH) <= 0){
				// compose_nameÇÔ¼ö·Î ºÒÇÊ¿äÇÑ °ø¹éÀ» Á¦°Å 
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
		// struct ÀÌ¹Ç·Î ÅëÂ°·Î ¿Å°ÜÁø´Ù.  
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
	
	ptr = strtok(NULL, " "); // ÀÌ¸§ÀÇ Ã¹¹øÂ° ´Ü¾î 
	if (ptr == NULL)
		return 0; // »ç¶÷ ÀÌ¸§ÀÇ ±æÀÌ°¡ 0 
	
	strcpy(str, ptr);
	length += strlen(ptr);
	
	while ((ptr = strtok(NULL, " ")) != NULL) {
		if (length + strlen(ptr)+1 < limit){
			// null char¸¦ À§ÇØ +1
			// ÀÌ¸§ÀÌ ³Ê¹« ±æ¾î¼­ BUFFER_LENGTH¸¦ ³Ñ±æ °æ¿ìÀÇ ¿¹¿ÜÃ³¸® 
			
			str[length++] = ' '; // ´Ü¾î¿Í ´Ü¾î»çÀÌ °ø¹éÀ»ÁÜ  
			str[length] = '\0';
			strcat(str, ptr);
			// '\0'À¸·Î ³¡³¯¶§¿¡¸¸ Á¤»óÀûÀ¸·Î ÀÛµ¿ 
			// ¾ÕÀÇ strtokÀ¸·Î »ý±ä '\0'¸¦ ' '°ø¹éÀ¸·Î ¹Ù²Ù¾úÀ¸¹Ç·Î ÇÊ¿äÇÏ´Ù.
			//strcat À¸·Î strÀÇ \0 null¹®ÀÚ À§Ä¡ºÎÅÍ ptr°¡ ÀÌ¾îºÙ´Â´Ù.Ù. 
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
	// Á¶°Ç ? A : B Á¶¸°ÇÀÌ ¸¸Á·µÇ¸é A¸¦ ÅÃÇÏ°í, ¾Æ´Ò°æ¿ì B¸¦ ÅÃÇÔ.
	 	
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
		// Á¸ÀçÇÏÁö ¾Ê´Â Ç×¸ñÀº °ø¹éÀ¸·Î ÀúÀåÇßÀ¸¹Ç·Î
		//Ç×¸ñÀÌ Á¸ÀçÇÏ´À³Ä ¿©ºÎ¸¦ µûÁöÁö ¾Ê¾Æµµ µÈ´Ù. 
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
		// ±¸Á¶Ã¼ º¯¼ö °£ÀÇ Ä¡È¯¿¬»êÀÌ Áö¿øµÇ¹Ç·Î ¸â¹ö Ç×¸ñµéÀ»
		// µû·Îµû·Î Ä¡È¯ÇÒ ÇÊ¿ä°¡ ¾ø´Ù. 
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



