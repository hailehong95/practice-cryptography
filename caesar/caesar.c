#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

void ShowMenu();
int KiemTraChuoi(char *);
void NhapChuoi(char *);
char* CaesarCipherEncode(char *, int);
char* CaesarCipherDecode(char *, int);

int main()
{
	ShowMenu();
	int choice;
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
	{
		int k = 5;
		char BanRo[100];
		NhapChuoi(BanRo);

		char *BanMa = CaesarCipherEncode(BanRo, k);
		printf("%s\n", BanMa);
		break;
	}
	case 2:
	{
		int k = 5; 
		char BanMa[100];
		NhapChuoi(BanMa);

		char *BanRo = CaesarCipherDecode(BanMa, k);
		printf("%s\n", BanRo);
		break;
	}
	default: printf("Nhap sai!"); break;
	}

	getch();
	return 0;
}

void ShowMenu()
{
	system("cls");
	printf("Caesar Cipher\n");
	printf("-------------\n");
	printf("1. Encoder\n");
	printf("2. Decoder\n");
	printf("-------------\n");
	printf("Ban chon: ");
}
int KiemTraChuoi(char *s)
{
	int n = strlen(s);
	char *str = strdup(s);
	strlwr(str);
	for (int i = 0; i < n; i++)
	{
		if (str[i] == 32) // Bỏ qua và không kiểm tra khoảng trắng (Space)
			continue;
		if (str[i] < 'a' || str[i] > 'z')
			return 0; // Chuỗi không hợp lệ
	}

	return 1; // Chuỗi hợp lệ
}

void NhapChuoi(char *s)
{
loop_input:
	printf("Nhap chuoi: ");
	rewind(stdin);
	gets(s);

	int check = KiemTraChuoi(s);
	if (check == 0)
	{
		printf("Nhap lai chuoi (a..z; A..Z)\n");
		goto loop_input;
	}
}

char* CaesarCipherEncode(char *BanRo, int key)
{
	int n = strlen(BanRo);
	char *str = (char *)malloc((n + 1) * sizeof(char));
	strlwr(BanRo);

	for (int i = 0; i < n; i++)
	{
		int tmp = ((BanRo[i] - 97) + key) % 26;
		if (BanRo[i] == ' ')	// Khoảng trắng
			str[i] = ' ';
		else
			str[i] = tmp + 97;
	}
	str[n] = '\0';

	return str;
}

char* CaesarCipherDecode(char *BanMa, int key)
{
	int n = strlen(BanMa);
	char *str = (char *)malloc((n + 1) * sizeof(char));
	strlwr(BanMa);

	for (int i = 0; i < n; i++)
	{
		int tmp = ((BanMa[i] - 97) - key) % 26;
		if (BanMa[i] == ' ')	// Khoảng trắng
			str[i] = ' ';
		else if (tmp < 0)
			str[i] = tmp + 97 + 26;
		else
			str[i] = tmp + 97;
	}
	str[n] = '\0';

	return str;
}