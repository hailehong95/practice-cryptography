#include <stdio.h>
#include <conio.h>
#include <string.h>

// Input là chuỗi các ký tự hợp lệ trong bảng mã ASCII: a..z; A..Z
// Output là chuỗi các ký tự sau khi đã dịch vòng theo bảng chữ cái

void ShowMenu();
int KiemTraChuoi(char *);
char* ROT(char *, int);

int main()
{
	ShowMenu();
	char s[100];

	printf("Nhap chuoi: ");
	gets(s);
	int check = KiemTraChuoi(s);	
	if (check)
	{
		printf("Ket qua:\n");
		for (int i = 0; i < 26; i++)
		{
			char *strRot = ROT(s, i);
			printf("ROT %d:\t %s\n", i, strRot);
		}
	}
	else
		printf("Chuoi khong hop le\n");

	getch();
	return 0;
}

void ShowMenu()
{
	printf("ROT Encode/Decode\n");
	printf("-----------------\n");
}
int KiemTraChuoi(char *s)
{
	int n = strlen(s);
	char *str = strdup(s);
	strlwr(str);
	for (int i = 0; i < n; i++)
	{
		if (str[i] == 32) // Space
			continue;
		if (str[i] < 'a' || str[i] > 'z')
			return 0;
	}

	return 1;
}
char* ROT(char *s, int x)
{
	char *str = strdup(s);
	int length = strlen(s);
	strlwr(str);
	for (int i = 0; i < length; i++)
	{
		int tmp = str[i] + x;
		if (str[i] == 32)
			continue;	// Bỏ qua khoảng trắng
		if (tmp > 122)	// Vượt qua khoảng a..z
			str[i] = str[i] + x - 26;
		else
			str[i] += x;
	}

	for (int i = 0; i < length; i++)
		if (s[i] >= 'A' && s[i] <= 'Z')
			str[i] -= 32;	// Khôi phục lại các ký tự in hoa

	return str;
}