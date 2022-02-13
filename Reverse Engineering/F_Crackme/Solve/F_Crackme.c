#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkUsername(char c[]) {
	int length;
	length = strlen(c);
	if(1 < length && length < 8) {
		return 0;
	}
	else if(length >= 8) {
		return 1;
	}
	else{
		return 2;
	}
}

void message() {
	printf("---------------------------------------------------\n");
	printf("Welcome to VishwaCTF Mini!! I hope you have fun\n");
	printf("Here's a simple problem to get you started!\n");
	printf("---------------------------------------------------\n\n");
}

int main() {
	char name[20];
	char password[30];
	int usernameType;
	char rightPasswd[30];

	message();
	printf("Type your username: ");
	scanf("%20s", name);
	usernameType = checkUsername(name);

	if(usernameType == 0){
		strcpy(rightPasswd, "2.615074");
		strcat(name, rightPasswd);
		strcpy(rightPasswd, name);
	}
	else if(usernameType == 1){
		strcpy(rightPasswd, "@Channeler-");
		strcat(rightPasswd, name);
	}
	else{
		exit(0);
	}

	printf("Type your password: ");
	scanf("%30s", password);

	int samePass = strcmp(rightPasswd, password);
	if(samePass == 0){
		printf("\nIf you got the references, vishwaCTF{El_Psy_Kongroo}");
	}
	else{
		printf("\nGo Back, Destroy SERN in an another worldline");
	}

	return 0;
}