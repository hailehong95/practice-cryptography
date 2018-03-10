#pragma warning(disable:4996)
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;

/*
* NOTE: BASE64 ĐANG DỞ, CHƯA HOÀN THÀNH
*/

void ShowMenu();
char * CharToBinary(char);
void AddAnCharacter(char *, int, char);
string StringToBinary(string);
string SixBitsBinaryToASCII_6Bits(string);
char * HexToBinary(char);
string StringHexToBinary(string);
char EightBitsBinaryToChar(string);
string Base64Encoder(string);
string HexDecoder(string);
string ChuanHoaChuoiNhiPhan(string);

int main()
{
	ShowMenu();
	int choice;
	cin >> choice;
	rewind(stdin);
	switch (choice)
	{
	case 1: // Encoder
	{
		string Plaintext, PlaintextBinary;
		cout << "Input: "; getline(cin, Plaintext);
		PlaintextBinary = StringToBinary(Plaintext);
		//ChuanHoaChuoiNhiPhan(PlaintextBinary);
		string test = Base64Encoder(PlaintextBinary);
		cout << test << endl;



		break;
	}
	case 2: // Decoder
	{

		break;
	}
	default: cout << "Nhap sai!" << endl; break;
	}

	system("pause");
	return 0;
}

void ShowMenu()
{
	cout << "Base64 Encode/Decode" << endl;
	cout << "-----------------" << endl;
	cout << "1. Base64 Encoder" << endl;
	cout << "2. Base64 Decoder" << endl;
	cout << "-----------------" << endl;
	cout << "Ban chon: ";
}
void AddAnCharacter(char *s, int vt, char gt)
{
	int n = strlen(s);
	for (int i = n - 1; i >= vt; i--)
		s[i + 1] = s[i];
	s[vt] = gt;
	s[n + 1] = '\0';
}
char * CharToBinary(char chr)
{
	char *bin = (char *)malloc(9 * sizeof(char));
	itoa(chr, bin, 2);

	// Chuẩn hóa cho đủ 8 bit dạng nhị phân
	if (strlen(bin) < 8)
	{
		int soLuong = 8 - strlen(bin);
		for (int i = 0; i < soLuong; i++)
			AddAnCharacter(bin, 0, '0');
	}
	return bin;
}
string StringToBinary(string s)
{
	string str;
	int nstr = s.length();

	for (int i = 0; i < nstr; i++)
	{
		char *tmp = CharToBinary(s.at(i));
		str += tmp;
		free(tmp);
	}

	return str;
}
string SixBitsBinaryToASCII_6Bits(string s)
{
	string Base64_ASCII = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string str;
	int sum = 0;
	int length = s.length();
	int somu = length - 1;

	for (int i = 0; i < length; i++)
		sum += (s.at(i) - '0') * pow(2, somu--);
	str = Base64_ASCII[sum];

	return str;
}
char * HexToBinary(char chr)
{
	// Chuyển 1 ký tự dạng hexa sang chuỗi 4 bit nhị phân
	char * str = (char *)malloc(5 * sizeof(char));
	if (chr >= '0' && chr <= '9')
		itoa((chr - '0'), str, 2);
	else
	{
		if (chr == 'a' || chr == 'A')
			itoa(10, str, 2);
		else if (chr == 'b' || chr == 'B')
			itoa(11, str, 2);
		else if (chr == 'c' || chr == 'C')
			itoa(12, str, 2);
		else if (chr == 'd' || chr == 'D')
			itoa(13, str, 2);
		else if (chr == 'e' || chr == 'E')
			itoa(14, str, 2);
		else if (chr == 'f' || chr == 'F')
			itoa(15, str, 2);
	}

	// Chuẩn hóa cho đủ 4 bits
	int ns = strlen(str);
	if (ns < 4)
	{
		int SoLuong = 4 - ns;
		for (int i = 0; i < SoLuong; i++)
			AddAnCharacter(str, 0, '0');
	}

	return str;
}
string StringHexToBinary(string s)
{
	string strBin;
	int ns = s.length();
	for (int i = 0; i < ns; i++)
	{
		char *tmp = HexToBinary(s.at(i));
		strBin += tmp;
		free(tmp);
	}

	return strBin;
}
char EightBitsBinaryToChar(string s)
{
	char chr = 0, length = s.length();
	int somu = length - 1;

	for (int i = 0; i < length; i++)
		chr += (s.at(i) - '0') * pow(2, somu--);

	return chr;
}
string Base64Encoder(string str)
{
	string Base64_ASCII = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string strBin = ChuanHoaChuoiNhiPhan(str);
	string Base64_String;
	int length = strBin.length();
	for (int i = 0; i < length; i += 6)
	{
		string shortBin = strBin.substr(i, 6);
		string shortBase64 = SixBitsBinaryToASCII_6Bits(shortBin);
		Base64_String += shortBase64;
	}

	return Base64_String;
}
string HexDecoder(string strBin)
{
	string str;
	int length = strBin.length();
	for (int i = 0; i < length; i += 8)
	{
		string shortBin = strBin.substr(i, 8);
		char shortStr = EightBitsBinaryToChar(shortBin);
		str += shortStr;
	}

	return str;
}
string ChuanHoaChuoiNhiPhan(string str)
{
	string strBin = str;
	uint64_t nstrBin = strBin.length();
	while (nstrBin % 24 != 0)
	{
		strBin.push_back('0');
		nstrBin++;
	}

	return strBin;
}