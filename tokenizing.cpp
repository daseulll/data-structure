#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char str[] = "now # is the time # to start preparing ### for the exam#";
	char delim[] = "#";
	char *token;
	
	token = strtok( str, delim );
	// (�迭���̸�(�����ּ�), delimeter) 
	// ���ϰ��� ù��° ��ū�� �����ּҸ� �����Ѵ�.
	// ���⼭�� n�� �ּҸ� ���´�.
	 
	while ( token != NULL ){
	// tokenizing�� ������ null�� �����Ѵ�. 
		printf("netx token is: %s:%d\n", token, strlen(token));
		token = strtok( NULL, delim );
		// �ι�°�� strtok�� ȣ���� �� ù�Ű������� NULL�� ����.
		// #�� �ڸ� �ι�° ���ڿ��� ù��° ��ū�� �����ּҸ� �����Ѵ�. 
		// �̿� ���� ����� ����. (���⼭ �ڼ��� �ٷ����� �ʴ´�.) 
	}
	return 0;
}
