#include <stdio.h>
#include <conio.h>
#include <string.h>

//----------XOR-----------
//Cipher = Plaintext ^ Key
//Plaintext = Cipher ^ Key
//------------------------

void ShowMenu();
char* XOR(char *, char *);

int main()
{
	ShowMenu();
	char s1[100], s2[] = { 'L','E','H','O','N','G','H','A','I' };
	printf("Nhap chuoi: ");
	gets(s1);

	strlwr(s1); //Fix Uper
	char *xorStr = XOR(s1, s2);
	printf("%s", xorStr);

	getch();
	return 0;
}

void ShowMenu()
{
	printf("XOR Encode/Decode\n");
	printf("-----------------\n");
}

char* XOR(char *s, char *key)
{
	char *str = strdup(s);
	int ns = strlen(s);

	for (int i = 0; i < ns; i++)
	{
		int x = i % strlen(key) / sizeof(char);
		str[i] = s[i] ^ key[x];
	}
	return str;
}