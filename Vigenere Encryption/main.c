#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

void ShowMenu();
void NhapBanRo(char *);
void NhapKhoa(char *);
void NhapBanMa(char *);
void XoaKyTu(char *, int);
void ThemKyTu(char *, int, char);
int KiemTraChuoi(char *);
char* KeyGenerator(char *, char *);
char* VigenereEncryption(char *, char *);
char* VigenereDecryption(char *, char *);

int main()
{
	ShowMenu();
	int choice;
	scanf("%d", &choice);

	switch (choice)
	{
	case 1:
	{
		char BanRo[100], Khoa[100];
		// Nhập bản rõ và khóa để mã hóa
		NhapBanRo(BanRo);
		NhapKhoa(Khoa);

		// Sinh khóa mới từ khóa người dùng nhập sao cho khóa mới bằng với chiều dài thông điệp
		char *KeyGen = KeyGenerator(BanRo, Khoa);

		// Tiến hành mã hóa 
		char *BanMa = VigenereEncryption(BanRo, KeyGen);

		// In kết quả sau khi mã hóa
		printf("Ban Ro: %s\n", BanRo);
		printf("Ban Ma: %s\n", BanMa);
		break;
	}
	case 2:
	{
		char BanMa[100], Khoa[100];
		// Nhập bản mã và khóa để giải mã
		NhapBanMa(BanMa);
		NhapKhoa(Khoa);

		// Sinh khóa mới từ khóa người dùng nhập sao cho khóa mới bằng với chiều dài thông điệp
		char *KeyGen = KeyGenerator(BanMa, Khoa);

		// Tiến hành giải mã
		char *BanRo = VigenereDecryption(BanMa, KeyGen);

		// In kết quả sau khi giải mã
		printf("Ban Ma: %s\n", BanMa);
		printf("Ban Ro: %s\n", BanRo);
		break;
	}
	default: printf("Nhap sai!\n"); break;
	}

	getch();
	return 0;
}

void ShowMenu()
{
	system("cls");
	printf("Vigenere Encode/Decode\n");
	printf("----------------------\n");
	printf("1. Vigenere Encoder\n");
	printf("2. Vigenere Decoder\n");
	printf("----------------------\n");
	printf("Ban chon: ");
}

void NhapKhoa(char *key)
{
loop_Key:
	printf("Nhap Khoa: ");
	gets(key);
	int KiemTraKhoa = KiemTraChuoi(key);
	if (KiemTraKhoa == 0)
	{
		printf("Nhap lai khoa (a..z; A..Z)\n");
		goto loop_Key;
	}
}

void NhapBanRo(char *banro)
{
loop_Banro:
	printf("Nhap Ban Ro (a..z; A..Z): ");
	rewind(stdin); // fflush(stdin);
	gets(banro);
	int KiemTraThongDiep = KiemTraChuoi(banro);
	if (KiemTraThongDiep == 0)
	{
		printf("Nhap lai thong diep (a..z; A..Z)\n");
		goto loop_Banro;
	}
}

void NhapBanMa(char *banma)
{
loop_Banma:
	printf("Nhap Ban ma (a..z; A..Z): ");
	rewind(stdin); // fflush(stdin);
	gets(banma);

	int KiemTraThongDiep = KiemTraChuoi(banma);
	if (KiemTraThongDiep == 0)
	{
		printf("Nhap lai ban ma (a..z; A..Z)\n");
		goto loop_Banma;
	}
}

// Hàm xóa 1 ký tự trong chuỗi
void XoaKyTu(char *s, int vt)
{
	int n = strlen(s);
	for (int i = vt + 1; i < n; i++)
		s[i - 1] = s[i];
	s[n - 1] = '\0'; // Dịch ký tự kết thúc chuỗi sang trái 1 ký tự
}

// Hàm thêm một ký tự trong chỗi
void ThemKyTu(char *s, int vt, char gt)
{
	int n = strlen(s);
	for (int i = n - 1; i >= vt; i--)
		s[i + 1] = s[i];
	s[vt] = gt;
	s[n + 1] = '\0'; // Dịch ký tự kết thúc chuỗi sang phải 1 ký tự
}
// Hàm kiểm tra chuỗi có hợp lệ hay không
// Chuỗi hợp lệ là các ký tự thuộc: A-Z, a-z
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

// Hàm sinh khóa mới sao cho có độ dài bằng với thông điệp
char* KeyGenerator(char *s1, char *s2)
{
	int ns1 = strlen(s1);
	int ns2 = strlen(s2);
	int n = 0;
	char *str = (char *)malloc((ns1 + 1) * sizeof(char));

	for (int i = 0; i < ns1; i++)
	{
		str[i] = s2[n++];
		if (n == ns2) n = 0;
	}
	str[ns1] = '\0';

	return str;
}

// Hàm mã hóa với đầu vào là bản rõ và khóa mã hóa
char* VigenereEncryption(char *PlainText, char *Key)
{
	char *strtmp = strdup(PlainText);

	// Loại bỏ khoảng trắng trong chuỗi
	int vt = 0;
	while (strtmp[vt] != '\0')
	{
		if (strtmp[vt] == ' ')
			XoaKyTu(strtmp, vt);
		vt++;
	}
	
	// Chuyển về cùng một kiểu chữ thường
	strlwr(strtmp);
	strlwr(Key);

	// Cấp phát chuỗi mới
	int n = strlen(strtmp);
	char *CipherText = (char *)malloc((n + 1) * sizeof(char));
	
	// Bắt đầu encode
	for (int i = 0; i < n; i++)
	{
		int tmp = strtmp[i] + (Key[i] - 97);

		if (tmp > 122)					// Vượt qua khoảng a..z
			CipherText[i] = tmp - 26;	// Dịch vòng quay lại a
		else
			CipherText[i] = tmp;

	}
	CipherText[n] = '\0';

	// Khôi phục lại vị trí các khoảng trắng
	int nPlain = strlen(PlainText);
	for (int i = 0; i < nPlain; i++)
	{
		if (PlainText[i] == ' ')
			ThemKyTu(CipherText, i, ' ');
	}

	return CipherText;
}

// Hàm giải mã với đầu vào là bản mã và khóa mã hóa
char* VigenereDecryption(char *CipherText, char *Key)
{
	char *strtmp = strdup(CipherText);

	// Loại bỏ khoảng trắng trong chuỗi
	int vt = 0;
	while (strtmp[vt] != '\0')
	{
		if (strtmp[vt] == ' ')
			XoaKyTu(strtmp, vt);
		vt++;
	}

	// Chuyển về cùng một kiểu chữ thường
	strlwr(strtmp);
	strlwr(Key);

	// Cấp phát chuỗi mới
	int n = strlen(strtmp);
	char *PlainText = (char *)malloc((n + 1) * sizeof(char));

	// Bắt đầu decode
	for (int i = 0; i < n; i++)
	{
		int tmp = strtmp[i] - (Key[i] - 97);

		if (tmp < 97)	// Vượt qua khoảng a..z
			PlainText[i] = tmp + 26; // Dịch vòng quay lại z
		else
			PlainText[i] = tmp;

	}
	PlainText[n] = '\0';

	// Khôi phục lại vị trí các khoảng trắng
	int nCipher = strlen(CipherText);
	for (int i = 0; i < nCipher; i++)
	{
		if (CipherText[i] == ' ')
			ThemKyTu(PlainText, i, ' ');
	}

	return PlainText;
}