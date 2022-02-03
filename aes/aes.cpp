#pragma warning(disable:4996)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;

int Nk = 8, Nr = 14;
unsigned char *key;
unsigned char state[4 * 4];			/* 128 bit */
unsigned char w[4 * (14 + 1) * 4];	/* max */
unsigned char invSBox[16 * 16];
unsigned char Sbox[16 * 16] =
{ // https://en.wikipedia.org/wiki/Rijndael_S-box
  /*    0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76, /*0*/
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0, /*1*/
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15, /*2*/
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75, /*3*/
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84, /*4*/
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF, /*5*/
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8, /*6*/
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, /*7*/
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73, /*8*/
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB, /*9*/
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79, /*a*/
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08, /*b*/
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A, /*c*/
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E, /*d*/
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF, /*e*/
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16  /*f*/
};
unsigned char Rcon[11 * 4] =
{
	/*	 0	   1	 2	   3	   */
	0x00, 0x00, 0x00, 0x00, /*0*/
	0x01, 0x00, 0x00, 0x00, /*1*/
	0x02, 0x00, 0x00, 0x00, /*2*/
	0x04, 0x00, 0x00, 0x00, /*3*/
	0x08, 0x00, 0x00, 0x00, /*4*/
	0x10, 0x00, 0x00, 0x00, /*5*/
	0x20, 0x00, 0x00, 0x00, /*6*/
	0x40, 0x00, 0x00, 0x00, /*7*/
	0x80, 0x00, 0x00, 0x00, /*8*/
	0x1b, 0x00, 0x00, 0x00, /*9*/
	0x36, 0x00, 0x00, 0x00  /*a*/
};
unsigned char gfMultBy02(unsigned char);
unsigned char gfMultBy03(unsigned char);
unsigned char gfMultBy09(unsigned char);
unsigned char gfMultBy0b(unsigned char);
unsigned char gfMultBy0d(unsigned char);
unsigned char gfMultBy0e(unsigned char);
void SetNkNr(int);
void InitInvSBox();
unsigned long RotWord(unsigned long);
unsigned long SubWord(unsigned long);
void ShiftRows();
void InvShiftRows();
void SubBytes();
void InvSubBytes();
void AddRoundKey(int);
void InvMixColumns();
void MixColumns();
void KeyExpantion(unsigned char *, unsigned char *);
void AES_Cipher(unsigned char *, unsigned char *);
void InvAES_Cipher(unsigned char *, unsigned char *);
void DisplayMenu();
void StringGenerator(unsigned char *, int);
void StringPadding(unsigned char *);
void StringSplit(unsigned char *, unsigned char *, int, int);
void StringMerge(unsigned char *, unsigned char *);
void StringInput(unsigned char *);
void ECB_Encryption(unsigned char *, unsigned char *, unsigned char *, int);
void ECB_Decryption(unsigned char *, unsigned char *, unsigned char *);
void CBC_Encryption(unsigned char *, unsigned char *, unsigned char *, int, unsigned char *);
void CBC_Decryption(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void ResetData(unsigned char *, unsigned char *, unsigned char *);

int main()
{
	unsigned char BanRo[100], BanMa[100] = "", BanDich[100] = "";
	unsigned char Khoa[35];

lb_start:
	DisplayMenu();

	int LuaChon;
	scanf("%d", &LuaChon);

	// AES 128
	if (LuaChon == 1)
	{
		StringGenerator(Khoa, 16);

		// ECB Mode
		ECB_Encryption(BanRo, Khoa, BanMa, 128);
		ECB_Decryption(BanMa, Khoa, BanDich);
		ResetData(BanRo, BanMa, BanDich);
		// CBC Mode
		unsigned char InitializationVector[17];
		StringGenerator(InitializationVector, 16);
		CBC_Encryption(BanRo, Khoa, BanMa, 128, InitializationVector);
		CBC_Decryption(BanMa, Khoa, BanDich, InitializationVector);
	}

	// AES 192
	if (LuaChon == 2)
	{
		StringGenerator(Khoa, 24);

		// ECB Mode
		ECB_Encryption(BanRo, Khoa, BanMa, 192);
		ECB_Decryption(BanMa, Khoa, BanDich);
		ResetData(BanRo, BanMa, BanDich);
		// CBC Mode
		unsigned char InitializationVector[17];
		StringGenerator(InitializationVector, 16);
		CBC_Encryption(BanRo, Khoa, BanMa, 192, InitializationVector);
		CBC_Decryption(BanMa, Khoa, BanDich, InitializationVector);
	}

	// AES 256
	if (LuaChon == 3)
	{
		StringGenerator(Khoa, 32);

		// ECB Mode
		ECB_Encryption(BanRo, Khoa, BanMa, 256);
		ECB_Decryption(BanMa, Khoa, BanDich);
		ResetData(BanRo, BanMa, BanDich);
		// CBC Mode
		unsigned char InitializationVector[17];
		StringGenerator(InitializationVector, 16);
		CBC_Encryption(BanRo, Khoa, BanMa, 256, InitializationVector);
		CBC_Decryption(BanMa, Khoa, BanDich, InitializationVector);
	}

	printf("\nBan co muon tiep tuc: Y/N? ");
	rewind(stdin);
	char c = getchar();
	if (c == 'y' || c == 'Y')
	{
		ResetData(BanRo, BanMa, BanDich);
		goto lb_start;
	}
	else
	{
		printf("Thoat!");
		ResetData(BanRo, BanMa, BanDich);
	}

	getch();
	return 0;
}

unsigned char gfMultBy02(unsigned char i)
{
	// Phep nhan tren truong GF(2^8)
	/* Thuc hien phep nhan 0x02 * a */
	if (i < 0x80)
		i = i << 1;
	else {
		i = i << 1;
		i = (unsigned char)((int)i ^ (int)0x1b);
		// i=(unsigned char)((int)i^(int)0x1b);
		// i=i<<1;
	}
	return i;
}
unsigned char gfMultBy03(unsigned char i)
{
	/* Thuc hien phep nhan 0x03 * a */
	return (unsigned char)((int)gfMultBy02(i) ^ (int)i);
}
unsigned char gfMultBy09(unsigned char i)
{
	/* Thuc hien phep nhan
	* 0x09 * a = (a * 0x02 * 0x02 * 0x02 ) + (a * 0x01)
	* - dung khi giai ma
	*/
	return (unsigned char)((int)gfMultBy02(gfMultBy02(gfMultBy02(i))) ^ (int)i);
}
unsigned char gfMultBy0b(unsigned char i)
{
	/* Thuc hien phep nhan 0x0b * a
	* 0x0b * a = (a * 0x02 * 0x02 * 0x02 ) + (a * 0x02) + (a * 0x01)
	* - dung khi giai ma
	*/
	return (unsigned char)((int)gfMultBy02(gfMultBy02(gfMultBy02(i))) ^ (int)gfMultBy02(i) ^ (int)i);
}
unsigned char gfMultBy0d(unsigned char i)
{
	/* Thuc hien phep nhan 0x0d * a
	* 0x0d * a = (a * 0x02 * 0x02 * 0x02 ) + (a * 0x02 * 0x02 ) + (a * 0x01)
	- dung khi giai ma*/
	return (unsigned char)((int)gfMultBy02(gfMultBy02(gfMultBy02(i))) ^
		(int)gfMultBy02(gfMultBy02(i)) ^ (int)(i));
}
unsigned char gfMultBy0e(unsigned char i)
{
	/* Thuc hien phep nhan 0x0e * a
	* 0x0e * a = (a * 0x02 * 0x02 * 0x02 ) + (a * 0x02 * 0x02 ) + (a * 0x02)
	- dung khi giai ma*/
	return (unsigned char)((int)gfMultBy02(gfMultBy02(gfMultBy02(i))) ^
		(int)gfMultBy02(gfMultBy02(i)) ^ (int)gfMultBy02(i));
}
void SetNkNr(int numkey)
{
	if ((numkey == 0) || (numkey == 128)) {
		Nk = 4;
		Nr = 10;
	}
	else if ((numkey == 1) || (numkey == 192)) {
		Nk = 6;
		Nr = 12;
	}
	else if ((numkey == 2) || (numkey == 256)) {
		Nk = 8;
		Nr = 14;
	}

	/* DEBUG */
	printf("\nNk = %d, Nr = %d ", Nk, Nr);
}
void InitInvSBox()
{
	/* Tao hop the nguoc voi Sbox - dung khi giai ma */
	FILE *f;

	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++) {
			unsigned char t;
			int x, y;
			t = Sbox[i * 16 + j];
			x = ((unsigned char)((int)(t) >> 4));
			y = ((unsigned char)((int)(t) & 0x0f));
			invSBox[x * 16 + y] = (unsigned char)((i << 4) + j);
		}

	f = fopen("invSbox.txt", "wb");
	fprintf(f, "\n Hop the hoan vi nguoc: ");
	fprintf(f, "\n invSBox[16*16] = {\n");

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			fprintf(f, "%02X, ", invSBox[i * 16 + j]);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "};");

	fclose(f);
}
unsigned long RotWord(unsigned long t)
{
	// Dich vong trai di 1 Byte
	unsigned char *p;
	unsigned char temp;

	p = (unsigned char *)&t;
	temp = *p;
	*p = *(p + 1);
	p++;
	*p = *(p + 1);
	p++;
	*p = *(p + 1);
	p++;
	*p = temp;

	return t;
}
void ShiftRows()
{
	// Dich trai cac dong (hang)
	unsigned char temp[4 * 4];
	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			temp[i * 4 + j] = state[i * 4 + j];
		}

	/* Vi dong 0 khong dich vong. Dong thu i dich di i byte */
	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			state[i * 4 + j] = temp[i * 4 + ((i + j) % 4)];
		}
}
void SubBytes()
{
	// Thay the mang 4*4 State thanh mang 4*4 qua Sbox.
	int x, y;
	for (int i = 0; i < 4; i++) /* cot */
		for (int j = 0; j < 4; j++) /* hang */ {
			/* a[i, j] = Sbox[a[i,j]] */
			// x= (unsigned char)((int)(state[i*4+j])>>4);
			// y= (unsigned char)((int)(state[i*4+j])& 0x0f);
			state[i * 4 + j] = Sbox[state[i * 4 + j]];
		}
}
unsigned long SubWord(unsigned long t)
{
	// Thay the qua Sbox. Thay the 4 byte. Dung trong luoc do tao khoa.
	unsigned char *p;
	int x, y;

	p = (unsigned char *)&t;
	x = (unsigned char)((int)(*p) >> 4);
	y = (unsigned char)((int)(*p) & 0x0f);
	*p = Sbox[x * 16 + y];

	p++;
	x = (unsigned char)((int)(*p) >> 4);
	y = (unsigned char)((int)(*p) & 0x0f);
	*p = Sbox[x * 16 + y];

	p++;
	x = (unsigned char)((int)(*p) >> 4);
	y = (unsigned char)((int)(*p) & 0x0f);
	*p = Sbox[x * 16 + y];

	p++;
	x = (unsigned char)((int)(*p) >> 4);
	y = (unsigned char)((int)(*p) & 0x0f);
	*p = Sbox[x * 16 + y];
	return t;
}
void KeyExpantion(unsigned char *key, unsigned char *w)
{
	/* Expantion userkey to Key Schedule */
	/*
	Mo rong tu 128 -> 4*(10+1)*4 = 1408 bit khoa con - 10 vong.
	Mo rong tu 192 -> 4*(12+1)*4 = 1664 bit khoa con - 12 vong.
	Mo rong tu 256 -> 4*(14+1)*4 = 2000 bit khoa con - 14 vong.

	Vi moi vong su dung Nb = 4*4 byte khoa. Can them 1 khoa con cho
	vong cuoi cung.241: */

	/* Nk*4 byte dau duoc lay tu khoa vao -> duoc Nk dong */
	for (int i = 0; i < Nk; i++) { /* theo dong */
		w[i * 4] = key[i * 4];
		w[i * 4 + 1] = key[i * 4 + 1];
		w[i * 4 + 2] = key[i * 4 + 2];
		w[i * 4 + 3] = key[i * 4 + 3];
	}

	/* Tu dong thu Nk den 4*(Nr+1)*4 */
	for (int row = Nk; row < (4 * (Nr + 1)); row++) {
		unsigned long temp = *((unsigned long*)&w[(row - 1) * 4]);/* W[i-1] = w[4*(i-1)]*/

		if (row%Nk == 0) {
			temp = SubWord(RotWord(temp)) ^ (*((unsigned long*)&Rcon[(row / Nk) * 11]));
		}
		else if (Nk > 6 && (row % Nk == 4)) {
			temp = SubWord(temp);
		}
		temp = temp ^ (*((unsigned long*)(&w[(row - Nk) * 4])));

		/* Ghi lai 4 byte nay vao w ung voi dong khoa thu row*/
		*((unsigned long*)&w[row * 4]) = temp;
	}
}
void AddRoundKey(int round)
{
	// Cong State voi khoa vong thu round
	for (int i = 0; i < 4; i++) /* cot */
		for (int j = 0; j < 4; j++) /* hang */ {
			state[i * 4 + j] = (unsigned char)((int)state[i * 4 + j] ^ (int)w[(round * 4 + j) * 4 + i]);
		}
}
void MixColumns()
{
	/* MixColumns */
	unsigned char temp[4 * 4];

	/* Copy du lieu tu State vao temp */
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			temp[i * 4 + j] = state[i * 4 + j];
		}

	//Ma tran goc === cot 1 va 2
	for (int i = 0; i < 4; i++) {
		state[0 * 4 + i] = (unsigned char)((int)gfMultBy02(temp[0 * 4 + i]) ^
			(int)gfMultBy03(temp[1 * 4 + i]) ^
			(int)temp[2 * 4 + i] ^
			(int)temp[3 * 4 + i]);

		state[1 * 4 + i] = (unsigned char)((int)temp[0 * 4 + i] ^
			(int)gfMultBy02(temp[1 * 4 + i]) ^
			(int)gfMultBy03(temp[2 * 4 + i]) ^
			(int)temp[3 * 4 + i]);

		state[2 * 4 + i] = (unsigned char)((int)temp[0 * 4 + i] ^ (int)temp[1 * 4 + i] ^
			(int)gfMultBy02(temp[2 * 4 + i]) ^
			(int)gfMultBy03(temp[3 * 4 + i]));

		state[3 * 4 + i] = (unsigned char)((int)gfMultBy03(temp[0 * 4 + i]) ^
			(int)temp[1 * 4 + i] ^
			(int)temp[2 * 4 + i] ^
			(int)gfMultBy02(temp[3 * 4 + i]));
	}
}
void AES_Cipher(unsigned char *input, unsigned char *output)
{
	/* 0. Neu khong co du lieu vao -> THOAT */
	if ((input == NULL) || (output == NULL))
		return;

	/* 1. Dua input vao State */
	for (int i = 0; i < 16; i++) { /* theo cot truoc */
		state[(i % 4) * 4 + (i / 4)] = input[i];
	}

	/* 2. AddRoundKey - vong thu 0*/
	// printf("\n round [0]. input ");
	//for (int i=0; i<16; i++) printf("%02X", state[(i%4)*4 + (i/4)]);
	AddRoundKey(0);
	//printf("\n round [0]. k_sch ");
	//for (int i=0; i<16; i++) printf("%02X", w[i]);


	/* 3. Thuc hien Nr vong lap */
	for (int round = 1; round <= (Nr - 1); ++round)
	{
		// printf("\n round [%d]. start ", round);
		//for (int i=0; i<16; i++) printf("%02X", state[(i%4)*4 + (i/4)]);

		SubBytes();

		//printf("\n round [%d]. s_box ", round);
		//for (int i=0; i<16; i++) printf("%02X", state[(i%4)*4 + (i/4)]);

		ShiftRows();

		//printf("\n round [%d]. s_row ", round);
		//for (int i=0; i<16; i++) printf("%02X", state[(i%4)*4 + (i/4)]);

		MixColumns();

		//printf("\n round [%d]. m_col ", round);
		//for (int i=0; i<16; i++) printf("%02X", state[(i%4)*4 + (i/4)]);

		AddRoundKey(round);

		//printf("\n round [%d]. k_sch ", round);
		//for (int i=0; i<16; i++) printf("%02X", w[(round)*16+i]);
	}

	/* 4. Final Round */
	SubBytes();
	ShiftRows(); AddRoundKey(Nr);

	/* Lay du lieu ra theo cot*/
	for (int i = 0; i < 16; i++) {
		output[i] = state[(i % 4) * 4 + (i / 4)];
	}
}
void InvShiftRows()
{
	/* Cac ham phuc vu qua trinh giai ma */
	/* Dich vong sang phai 1 byte */
	unsigned char temp[4 * 4];

	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			temp[i * 4 + j] = state[i * 4 + j];
		}

	/* lay nguoc cua phep bien doi o ShiftRows o tren */
	for (int i = 1; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			state[i * 4 + ((i + j) % 4)] = temp[i * 4 + j];
		}
}
void InvSubBytes()
{
	/* Thuc hien phep the nguoc - thong qua bang the nguoc invSBox */
	int x, y;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			// x= (unsigned char)((int)(state[i*4+j])>>4);
			// y= (unsigned char)((int)(state[i*4+j])&0x0f);
			state[i * 4 + j] = invSBox[state[i * 4 + j]];
		}
}
void InvMixColumns()
{
	unsigned char temp[4 * 4];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			temp[i * 4 + j] = state[i * 4 + j];
		}
	//ma tran goc
	for (int i = 0; i < 4; i++) {


		state[0 * 4 + i] = (unsigned char)((int)gfMultBy0e(temp[0 * 4 + i]) ^
			(int)gfMultBy0b(temp[1 * 4 + i]) ^
			(int)gfMultBy0d(temp[2 * 4 + i]) ^
			(int)gfMultBy09(temp[3 * 4 + i]));


		state[1 * 4 + i] = (unsigned char)((int)gfMultBy09(temp[0 * 4 + i]) ^
			(int)gfMultBy0e(temp[1 * 4 + i]) ^
			(int)gfMultBy0b(temp[2 * 4 + i]) ^
			(int)gfMultBy0d(temp[3 * 4 + i]));


		state[2 * 4 + i] = (unsigned char)((int)gfMultBy0d(temp[0 * 4 + i]) ^ (int)gfMultBy09(temp[1 * 4 + i]) ^
			(int)gfMultBy0e(temp[2 * 4 + i]) ^
			(int)gfMultBy0b(temp[3 * 4 + i]));


		state[3 * 4 + i] = (unsigned char)((int)gfMultBy0b(temp[0 * 4 + i]) ^
			(int)gfMultBy0d(temp[1 * 4 + i]) ^
			(int)gfMultBy09(temp[2 * 4 + i]) ^
			(int)gfMultBy0e(temp[3 * 4 + i]));
	}
}
void InvAES_Cipher(unsigned char *input, unsigned char *output)
{
	// Ham thuc hien giai ma AES
	/* Neu input hoac output chua khoi tao -> THOAT */
	if (input == NULL || output == NULL)
		return;

	/* 1. Dua input vao State */
	for (int i = 0; i < 16; i++) { /* theo cot */
		state[(i % 4) * 4 + (i / 4)] = input[i];
	}

	/* 2. AddRoundKey - Khoa cua vong thu Nr*/
	AddRoundKey(Nr);

	/* 3. Thuc hien Nr vong lap nguoc */
	for (int round = Nr - 1; round >= 1; --round) {
		InvShiftRows();
		InvSubBytes();
		AddRoundKey(round);
		InvMixColumns();
	}

	/* 4. Final Round */
	InvShiftRows();
	InvSubBytes();
	AddRoundKey(0);

	/* Lay du lieu ra - theo cot*/
	for (int i = 0; i < 16; i++) {
		output[i] = state[(i % 4) * 4 + (i / 4)];
	}
}
void DisplayMenu()
{
	printf("Ma hoa - Giai ma voi AES");
	printf("\n------------------------\n");
	printf("    1. AES 128\n");
	printf("    2. AES 192\n");
	printf("    3. AES 256\n");
	printf("Ban chon: ");
}
void StringGenerator(unsigned char *buff, int strLen)
{
	srand(time(0));
	for (int i = 0; i < strLen; i++)
		buff[i] = (unsigned char)(rand() % 255);
	buff[strLen] = '\0';
}
void StringPadding(unsigned char *buff)
{
	int buffLen = strlen((char *)buff);
	int chrCount = 0;
	while (buffLen % 16 != 0) // AES: 1 block = 128 bit = 16 characters
	{
		buff[buffLen++] = '0';
		chrCount++;
	}
	buff[buffLen - 1] = chrCount;
	buff[buffLen] = '\0';
}
void StringSplit(unsigned char *input, unsigned char *output, int start, int end)
{
	// Hàm tách ra một chuỗi con từ một chuỗi lớn.
	int idx = 0;
	for (int i = start; i <= end; i++)
		output[idx++] = input[i];
	output[idx] = '\0';
}
void StringMerge(unsigned char *s1, unsigned char *s2)
{
	// Hàm nối chuối s2 vào sau chuỗi s1
	int idx = strlen((char *)s1);
	for (int i = 0; i < 16; i++) // 16 character = 128 bits = 1 block
		s1[idx++] = s2[i];
	s1[idx] = '\0';
}
void StringInput(unsigned char *buff)
{
	string str; rewind(stdin);
	printf("Nhap ban ro: "); getline(cin, str);
	int length = str.length();
	for (int i = 0; i < length; i++)
		buff[i] = str[i];
	buff[length] = '\0';
}
void ECB_Encryption(unsigned char *BanRo, unsigned char *Khoa, unsigned char *BanMa, int KeyLength)
{	// ECB: Electronic Codebook Mode

	// Nhập bản rõ từ người dùng
	printf("\nECB Mode\n");
	StringInput(BanRo);

	int ChieuDaiTruocKhiDem = strlen((char *)BanRo);
	printf("\nChieu dai ban ro: %d", ChieuDaiTruocKhiDem);

	// Chuẩn hóa bản rõ.
	if (ChieuDaiTruocKhiDem % 16 != 0) StringPadding(BanRo);

	int ChieuDaiSauKhiDem = strlen((char *)BanRo);
	printf("\nSo byte can dem them: %d", ChieuDaiSauKhiDem - ChieuDaiTruocKhiDem);

	printf("\nChuoi dem (hex): ");
	for (int i = ChieuDaiTruocKhiDem; i < ChieuDaiSauKhiDem; i++)
		printf("%02X", BanRo[i]);

	printf("\nBan ro trc khi dem (hex): ");
	for (int i = 0; i < ChieuDaiTruocKhiDem; i++)
		printf("%02X", BanRo[i]);

	printf("\nBan ro sau khi dem (hex): ");
	for (int i = 0; i < ChieuDaiSauKhiDem; i++) printf("%02X", BanRo[i]);

	// Sinh khóa ngẫu nhiên
	/*if (KeyLength == 192)
	StringGenerator(Khoa, 24);
	else if (KeyLength == 256)
	StringGenerator(Khoa, 32);
	else
	StringGenerator(Khoa, 16);*/

	printf("\nKhoa dung de ma hoa (hex): ");
	int ChieuDaiKhoa = strlen((char *)Khoa);
	for (int i = 0; i < ChieuDaiKhoa; i++)
		printf("%02X", Khoa[i]);

	// Encryption
	SetNkNr(KeyLength); // 128, 192, 256

						// Tính số khối bản rõ
	int SoKhoiBanRo = strlen((char *)BanRo) / 16;
	KeyExpantion(Khoa, w);
	printf("\n\nStarting Encrypt...");

	int start_enc = 0;
	for (int i = 0; i < SoKhoiBanRo; i++)
	{
		unsigned char tmp_bro_en[17] = "";
		unsigned char tmp_bma_en[17] = "";

		int end_enc = start_enc + 16;
		StringSplit(BanRo, tmp_bro_en, start_enc, end_enc - 1);
		// Mã hóa từng khối
		AES_Cipher(tmp_bro_en, tmp_bma_en);
		tmp_bma_en[16] = '\0';
		// Nối từng khối bản mã thành bản mã hoàn chỉnh
		StringMerge(BanMa, tmp_bma_en);

		// Chuyển khối
		start_enc += 16;
	}

	printf("\nBan ma (ECB Mode): ");
	int ChieuDaiBanMa = strlen((char *)BanMa);
	for (int i = 0; i < ChieuDaiBanMa; i++)
		printf("%02X", BanMa[i]);
}
void ECB_Decryption(unsigned char *BanMa, unsigned char *Khoa, unsigned char *BanDich)
{	// ECB: Electronic Codebook Mode

	// Decryption
	InitInvSBox();
	KeyExpantion(Khoa, w);
	printf("\n\nStarting Decrypt...");

	// Tính số khổi của bản mã
	int SoKhoiBanMa = strlen((char *)BanMa) / 16;

	int start_dec = 0;
	for (int i = 0; i < SoKhoiBanMa; i++)
	{
		unsigned char tmp_bdich_de[17] = "";
		unsigned char tmp_bma_de[17] = "";

		int end_dec = start_dec + 16;
		StringSplit(BanMa, tmp_bma_de, start_dec, end_dec - 1);
		// Giải mã từng khối
		InvAES_Cipher(tmp_bma_de, tmp_bdich_de);
		tmp_bdich_de[16] = '\0';
		// Nối từng khối bản mã thành bản mã hoàn chỉnh
		StringMerge(BanDich, tmp_bdich_de);

		// Chuyển khối
		start_dec += 16;
	}

	printf("\nBan dich + Padding (hex) (ECB Mode): ");
	// Chiều dài bản dịch tính cả đệm
	int ChieuDaiBanDich = strlen((char *)BanDich);
	for (int i = 0; i < ChieuDaiBanDich; i++)
		printf("%02X", BanDich[i]);

	// Chiều dài bản dịch sau khi loại bỏ đệm
	int ChieuDaiChuoiDem = BanDich[ChieuDaiBanDich - 1];
	printf("\nBan dich (hex) (ECB Mode): ");
	int ChieuDaiBanDau = ChieuDaiBanDich - ChieuDaiChuoiDem;
	for (int i = 0; i < ChieuDaiBanDau; i++)
		printf("%02X", BanDich[i]);
}
void CBC_Encryption(unsigned char *BanRo, unsigned char *Khoa, unsigned char *BanMa, int KeyLength, unsigned char *InitializationVector)
{	// CBC: Cipher Bock Chaining Mode

	// Nhập bản rõ từ người dùng
	printf("\n\nCBC Mode\n");
	StringInput(BanRo);

	int ChieuDaiTruocKhiDem = strlen((char *)BanRo);
	printf("\nChieu dai ban ro: %d", ChieuDaiTruocKhiDem);

	// Chuẩn hóa bản rõ.
	if (ChieuDaiTruocKhiDem % 16 != 0) StringPadding(BanRo);

	int ChieuDaiSauKhiDem = strlen((char *)BanRo);
	printf("\nSo byte can dem them: %d", ChieuDaiSauKhiDem - ChieuDaiTruocKhiDem);

	printf("\nChuoi dem (hex): ");
	for (int i = ChieuDaiTruocKhiDem; i < ChieuDaiSauKhiDem; i++)
		printf("%02X", BanRo[i]);

	printf("\nBan ro trc khi dem (hex): ");
	for (int i = 0; i < ChieuDaiTruocKhiDem; i++)
		printf("%02X", BanRo[i]);

	printf("\nBan ro sau khi dem (hex): ");
	for (int i = 0; i < ChieuDaiSauKhiDem; i++) printf("%02X", BanRo[i]);

	// Sinh khóa ngẫu nhiên
	/*if (KeyLength == 192)
	StringGenerator(Khoa, 24);
	else if (KeyLength == 256)
	StringGenerator(Khoa, 32);
	else
	StringGenerator(Khoa, 16);*/

	printf("\nKhoa dung de ma hoa (hex): ");
	int ChieuDaiKhoa = strlen((char *)Khoa);
	for (int i = 0; i < ChieuDaiKhoa; i++)
		printf("%02X", Khoa[i]);

	// Encryption
	SetNkNr(KeyLength); // 128, 192, 256

						// Tính số khối bản rõ
	int SoKhoiBanRo = strlen((char *)BanRo) / 16;
	KeyExpantion(Khoa, w);
	printf("\n\nStarting Encrypt...");

	int start_enc = 0;
	for (int i = 0; i < SoKhoiBanRo; i++)
	{
		unsigned char tmp_bro_en[17];
		unsigned char tmp_bma_en[17];

		int end_enc = start_enc + 16;
		StringSplit(BanRo, tmp_bro_en, start_enc, end_enc - 1);

		// --- Start CBC Mode --- //
		if (i == 0) // Khối đầu tiên được xor với Init..Vector rồi mới mã hóa
		{
			//int length = strlen((char *)InitializationVector);
			for (int i = 0; i < 16; i++)
				tmp_bro_en[i] ^= InitializationVector[i];
		}
		else // Khối thứ 2 trở đi sẽ đc xor với bản mã của khối trước đó
		{
			//int length = strlen((char *)tmp_bro_en);
			for (int i = 0; i < 16; i++)
				tmp_bro_en[i] ^= tmp_bma_en[i];
		}
		// --- End CBC Mode --- //

		// Mã hóa từng khối
		AES_Cipher(tmp_bro_en, tmp_bma_en);
		tmp_bma_en[16] = '\0';
		// Nối từng khối bản mã thành bản mã hoàn chỉnh
		StringMerge(BanMa, tmp_bma_en);

		// Chuyển khối
		start_enc += 16;
	}

	printf("\nBan ma (CBC Mode):");
	int ChieuDaiBanMa = strlen((char *)BanMa);
	for (int i = 0; i < ChieuDaiBanMa; i++)
		printf("%02X", BanMa[i]);
}
void CBC_Decryption(unsigned char *BanMa, unsigned char *Khoa, unsigned char *BanDich, unsigned char *InitializationVector)
{	// CBC: Cipher Bock Chaining Mode
	unsigned char BanMaCuaKhoiTruocDo[17];

	// Decryption
	InitInvSBox();
	KeyExpantion(Khoa, w);
	printf("\n\nStarting Decrypt...");

	// Tính số khổi của bản mã
	int SoKhoiBanMa = strlen((char *)BanMa) / 16;

	int start_dec = 0;
	for (int i = 0; i < SoKhoiBanMa; i++)
	{
		unsigned char tmp_bdich_de[17] = "";
		unsigned char tmp_bma_de[17] = "";
		//int flag = 0;

		int end_dec = start_dec + 16;
		StringSplit(BanMa, tmp_bma_de, start_dec, end_dec - 1);

		// Giải mã từng khối
		InvAES_Cipher(tmp_bma_de, tmp_bdich_de);
		tmp_bdich_de[16] = '\0';

		// --- Start CBC Mode --- //

		if (i == 0) // Bản dịch của khối đầu tiên được xor với Init..Vector
		{
			for (int i = 0; i < 16; i++)
				tmp_bdich_de[i] ^= InitializationVector[i];
		}
		else // Khối thứ 2 trở đi được xor với bản mã của khối trước đó
		{
			for (int i = 0; i < 16; i++)
				tmp_bdich_de[i] ^= BanMaCuaKhoiTruocDo[i];
		}

		// Lưu khối bản mã ra một bản copy
		for (int i = 0; i < 17; i++)
			BanMaCuaKhoiTruocDo[i] = tmp_bma_de[i];

		// --- End CBC Mode --- //

		// Nối từng khối bản mã thành bản mã hoàn chỉnh
		StringMerge(BanDich, tmp_bdich_de);

		// Chuyển khối
		start_dec += 16;
	}

	printf("\nBan dich + Padding (hex) (CBC Mode): ");
	// Chiều dài bản dịch tính cả đệm
	int ChieuDaiBanDich = strlen((char *)BanDich);
	for (int i = 0; i < ChieuDaiBanDich; i++)
		printf("%02X", BanDich[i]);

	// Chiều dài bản dịch sau khi loại bỏ đệm
	int ChieuDaiChuoiDem = BanDich[ChieuDaiBanDich - 1];
	printf("\nBan dich (hex) (CBC Mode): ");
	int ChieuDaiBanDau = ChieuDaiBanDich - ChieuDaiChuoiDem;
	for (int i = 0; i < ChieuDaiBanDau; i++)
		printf("%02X", BanDich[i]);
}
void ResetData(unsigned char *BanRo, unsigned char *BanMa, unsigned char *BanDich)
{
	int ChieuDaiBanRo = strlen((char *)BanRo);
	for (int i = 0; i < ChieuDaiBanRo; i++)
		BanRo[i] = (unsigned char)0;
	BanRo[0] = '\0';

	int ChieuDaiBanMa = strlen((char *)BanMa);
	for (int i = 0; i < ChieuDaiBanMa; i++)
		BanMa[i] = (unsigned char)0;
	BanMa[0] = '\0';

	int ChieuDaiBanDich = strlen((char *)BanDich);
	for (int i = 0; i < ChieuDaiBanDich; i++)
		BanDich[i] = (unsigned char)0;
	BanDich[0] = '\0';
}