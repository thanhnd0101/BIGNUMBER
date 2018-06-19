#include "qint.h"
#include "share.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool la_hop_le_QInt(char *num, int radix)
{
	if (!num || strlen(num) <= 0)
		return 0;
	int i = 0;
	if (radix == 10) {
		if (num[i] == '+' || num[i] == '-')
			++i;
	}
	for (; num[i] != '\0'; ++i) {
		if (!la_chu_so(num[i], radix))
			return 0;
	}
	return 1;
}

bool *str2_to_bit(char *num, int size)
{
	bool *ans = (bool *)malloc(sizeof(bool) * size);
	int len = strlen(num); // Dem do dai cua chuoi
	for (int i = size - 1; i >= size - len; i--)
		ans[i] = num[len + i - size] -
			 '0'; // Sao chep chuoi vao cuoi day bit
	for (int i = 0; i < size - len; i++)
		ans[i] = 0; // Dat cac bit con trong la 0
	return ans;
}

char *bit_to_str2(bool *str2, int size)
{
	int count = 0;
	while (str2[count] == 0 && count < size)
		count++;	    // Dem so bit 0 o dau day bit
	int len_str = size - count; // Do dai cua chuoi ket qua
	char *ans = NULL;
	if (len_str == 0) {
		ans = (char *)malloc(sizeof(char) * 2);
		ans[0] = '0';
		ans[1] = '\0';
	} else {
		ans = (char *)malloc(sizeof(char) * (len_str + 1));
		ans[len_str] = '\0';
		for (int i = len_str - 1; i >= 0; i--)
			if (str2[i + size - len_str] == 1)
				ans[i] = '1';
			else
				ans[i] = '0';
	}
	return ans;
}

bool *str16_to_bit(char *num, int size)
{
	bool *bits = (bool *)malloc(sizeof(bool) * size);
	int len = strlen(num);
	int temp = size - 1;
	for (int i = len - 1; i >= 0 && temp >= 0; --i) {
		if (num[i] == '0') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 0;
			bits[temp - 1] = 0;
			bits[temp] = 0;
		} else if (num[i] == '1') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 0;
			bits[temp - 1] = 0;
			bits[temp] = 1;
		} else if (num[i] == '2') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 0;
			bits[temp - 1] = 1;
			bits[temp] = 0;
		} else if (num[i] == '3') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 0;
			bits[temp - 1] = 1;
			bits[temp] = 1;
		} else if (num[i] == '4') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 1;
			bits[temp - 1] = 0;
			bits[temp] = 0;
		} else if (num[i] == '5') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 1;
			bits[temp - 1] = 0;
			bits[temp] = 1;
		} else if (num[i] == '6') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 1;
			bits[temp - 1] = 1;
			bits[temp] = 0;
		} else if (num[i] == '7') {
			bits[temp - 3] = 0;
			bits[temp - 2] = 1;
			bits[temp - 1] = 1;
			bits[temp] = 1;
		} else if (num[i] == '8') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 0;
			bits[temp - 1] = 0;
			bits[temp] = 0;
		} else if (num[i] == '9') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 0;
			bits[temp - 1] = 0;
			bits[temp] = 1;
		} else if (num[i] == 'A') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 0;
			bits[temp - 1] = 1;
			bits[temp] = 0;
		} else if (num[i] == 'B') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 0;
			bits[temp - 1] = 1;
			bits[temp] = 1;
		} else if (num[i] == 'C') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 1;
			bits[temp - 1] = 0;
			bits[temp] = 0;
		} else if (num[i] == 'D') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 1;
			bits[temp - 1] = 0;
			bits[temp] = 1;
		} else if (num[i] == 'E') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 1;
			bits[temp - 1] = 1;
			bits[temp] = 0;
		} else if (num[i] == 'F') {
			bits[temp - 3] = 1;
			bits[temp - 2] = 1;
			bits[temp - 1] = 1;
			bits[temp] = 1;
		}
		temp -= 4;
	}

	// cac bit con trong
	if (temp >= 0) {
		for (int i = 0; i <= temp; ++i) {
			bits[i] = 0;
		}
	}

	return bits;
}

char *bit_to_str16(bool *bits)
{
	QInt q = BinToDec_int(bits);
	char *hexas = DecToHex_int(q);
	return hexas;
}

// Nhap xuat theo YEUCAU
void ScanQInt(QInt &q)
{
	const int max_size = 50;
	char num[max_size + 1];
	printf("Nhap so nguyen lon: ");
	scanf("%s", num);
	bool *bits;
	if (!la_hop_le_QInt(num, 10)) {
		printf("Input khong hop le\n");
		bits = (bool *)malloc(sizeof(bool) * QInt_Size);
		for (int i = 0; i < QInt_Size; ++i) {
			bits[i] = 0;
		}
	} else {
		bits = str10_to_bit(num, QInt_Size);
	}
	q = BinToDec_int(bits);
	free(bits);
}

void PrintQInt(QInt q)
{
	bool *bits = DecToBin_int(q);
	printf("Xuat so nguyen lon: ");
	char *str10 = bit_to_str10(bits, 128);
	printf("%s\n", str10);
	free(bits);
	free(str10);
}

// Chuyen doi theo YEUCAU, mac dinh bits la 128
bool *DecToBin_int(QInt q)
{
	bool *bits = (bool *)malloc(sizeof(bool) * 128);
	for (int i = 0, y = 0; i < 4; ++i, y += 32) {
		int_to_seq(q.block[i], bits, y, y + 31);
	}
	return bits;
}

QInt BinToDec_int(bool *bits)
{
	QInt x;
	for (int i = 0, j = 0; i < 4; ++i, j += 32) {
		x.block[i] = seq_to_int(bits, j, j + 31);
	}
	return x;
}

char *BinToHex_int(bool *bits)
{
	// cu 4 bits la 1 chu so hexa
	// 128 : 4 = 32
	// chuyen doi tung nibble (4-bit)
	const int size = 32;
	char *hexas = (char *)malloc(sizeof(char) * (size + 1));
	hexas[size] = '\0';
	char hexa_digit[] = "0123456789ABCDEF";
	for (int i = 0, j = 0; i < size; ++i, j += 4) {
		int d = seq_to_uint(bits, j, j + 3);
		hexas[i] = hexa_digit[d];
	}
	return hexas;
}

char *DecToHex_int(QInt q)
{
	bool *temp_bits = DecToBin_int(q);
	bool bits[128];
	for (int i = 0; i < 128; ++i) {
		bits[i] = temp_bits[i];
	}
	free(temp_bits);
	return BinToHex_int(bits);
}

/* Xu ly toan tu + - * /
 * tran so cua cong: am + am = duong va duong + duong = am
 * tru la nguoc lai cua cong
 * nhan su dung booth
 */
QInt operator+(QInt a, QInt b)
{
	bool *bits_1 = DecToBin_int(a);
	bool *bits_2 = DecToBin_int(b);
	const int size = 128;
	bool *tong = cong_bits(bits_1, bits_2, size);
	QInt q = BinToDec_int(tong);
	free(bits_1);
	free(bits_2);
	free(tong);
	return q;
}

QInt operator-(QInt a, QInt b)
{
	// tru la cong voi so doi
	bool *bits_2 = DecToBin_int(b);
	doi_dau_bit(bits_2, 128);
	QInt new_b = BinToDec_int(bits_2);
	free(bits_2);
	return a + new_b;
}

// nhan bang thuat toan booth
// A  Q  Q0  M
// 0  a  0   .
QInt operator*(QInt Q, QInt M)
{
	bool *bits_1 = DecToBin_int(Q);
	bool *bits_2 = DecToBin_int(M);
	bool *tich = nhan_bits(bits_1, bits_2, QInt_Size);
	QInt result = BinToDec_int(tich);
	free(bits_1);
	free(bits_2);
	free(tich);
	return result;
}

// Chia bang thuat toan chia khong dau
// A  Q           M
// 0  so bi chia  so chia
QInt operator/(QInt Q, QInt M)
{
	bool *Q_bits = DecToBin_int(Q);
	bool *M_bits = DecToBin_int(M);
	bool *r;
	bool *Quotient = chia_bits(Q_bits, M_bits, QInt_Size, r);
	QInt q = BinToDec_int(Quotient);
	free(Q_bits);
	free(M_bits);
	free(r);
	free(Quotient);
	return q;
}

/* Xu ly toan tu AND (&), OR(|), XOR(^), NOT(~)
 * dich trai(<<) va dich phai(>>)
 */
QInt operator&(QInt a, QInt b)
{
	bool *bits_1 = DecToBin_int(a);
	bool *bits_2 = DecToBin_int(b);
	bool *bits_3 = (bool *)malloc(sizeof(bool) * 128);

	for (int i = 0; i < 128; ++i) {
		if (bits_1[i] == 1 && bits_2[i] == 1)
			bits_3[i] = 1;
		else
			bits_3[i] = 0;
	}

	QInt q = BinToDec_int(bits_3);

	free(bits_1);
	free(bits_2);
	free(bits_3);
	return q;
}

QInt operator|(QInt a, QInt b)
{
	bool *bits_1 = DecToBin_int(a);
	bool *bits_2 = DecToBin_int(b);
	bool *bits_3 = (bool *)malloc(sizeof(bool) * 128);

	for (int i = 0; i < 128; ++i) {
		if (bits_1[i] == 0 && bits_2[i] == 0)
			bits_3[i] = 0;
		else
			bits_3[i] = 1;
	}

	QInt q = BinToDec_int(bits_3);

	free(bits_1);
	free(bits_2);
	free(bits_3);
	return q;
}

QInt operator^(QInt a, QInt b)
{
	bool *bits_1 = DecToBin_int(a);
	bool *bits_2 = DecToBin_int(b);
	bool *bits_3 = (bool *)malloc(sizeof(bool) * 128);

	for (int i = 0; i < 128; ++i) {
		if (bits_1[i] == bits_2[i])
			bits_3[i] = 0;
		else
			bits_3[i] = 1;
	}

	QInt q = BinToDec_int(bits_3);

	free(bits_1);
	free(bits_2);
	free(bits_3);
	return q;
}

QInt operator~(QInt a)
{
	bool *bits_1 = DecToBin_int(a);
	nghich_dao_bit(bits_1, 128);
	QInt q = BinToDec_int(bits_1);
	free(bits_1);
	return q;
}

QInt operator<<(QInt a, int count)
{
	const int size = 128;
	bool *bits = DecToBin_int(a);
	if (count >= size) {
		for (int i = 0; i < size; ++i) {
			bits[i] = 0;
		}
	} else {
		for (int i = 0; i < count; ++i) {
			dich_trai_1_bit(bits, size);
		}
	}
	QInt q = BinToDec_int(bits);
	free(bits);
	return q;
}

QInt operator>>(QInt a, int count)
{
	const int size = 128;
	bool *bits = DecToBin_int(a);
	if (count >= size) {
		for (int i = 1; i < size; ++i) {
			bits[i] = bits[0];
		}
	} else {
		for (int i = 0; i < count; ++i) {
			dich_phai_1_bit(bits, size);
		}
	}
	QInt q = BinToDec_int(bits);
	free(bits);
	return q;
}

// Cac ham kiem tra
void test_input_convert_int()
{
	printf("Test input output int\n");
	QInt q;
	ScanQInt(q);
	PrintQInt(q);

	printf("DecToBin_int\n");
	bool *bits = DecToBin_int(q);
	in_bit(bits, 128);

	printf("BinToDec_int\n");
	QInt q_2 = BinToDec_int(bits);
	PrintQInt(q_2);

	printf("BinToHex_int\n");
	char *s1 = BinToHex_int(bits);
	printf("%s\n", s1);
	free(s1);

	printf("DecToHex_int\n");
	char *s2 = DecToHex_int(q);
	printf("%s\n", s2);
	free(s2);

	free(bits);
}

void test_cong_tru_int()
{
	printf("Test cong tru int\n");
	QInt q_1, q_2;
	ScanQInt(q_1);
	ScanQInt(q_2);

	printf("Tong\n");
	QInt q_3 = q_1 + q_2;
	PrintQInt(q_3);

	printf("Hieu\n");
	QInt q_4 = q_1 - q_2;
	PrintQInt(q_4);
}

void test_nhan_chia_int()
{
	printf("Test nhan chia int\n");
	QInt q_1, q_2;
	ScanQInt(q_1);
	ScanQInt(q_2);

	printf("Nhan\n");
	QInt q_3 = q_1 * q_2;
	PrintQInt(q_3);

	printf("Chia\n");
	QInt q_4 = q_1 / q_2;
	PrintQInt(q_4);
}

void test_bit_operator_int()
{
	printf("Test bit operator int\n");
	QInt q_1, q_2;
	ScanQInt(q_1);
	ScanQInt(q_2);

	printf("AND\n");
	QInt q_3 = q_1 & q_2;
	PrintQInt(q_3);

	printf("OR\n");
	QInt q_4 = q_1 | q_2;
	PrintQInt(q_4);

	printf("XOR\n");
	QInt q_5 = q_1 ^ q_2;
	PrintQInt(q_5);

	printf("NOT\n");
	QInt q_6 = ~q_1;
	QInt q_7 = ~q_2;
	PrintQInt(q_6);
	PrintQInt(q_7);

	printf("Left Shift\n");
	QInt q_8 = q_1 << 1;
	QInt q_9 = q_2 << 2;
	PrintQInt(q_8);
	PrintQInt(q_9);

	printf("Right Shift\n");
	QInt q_10 = q_1 >> 1;
	QInt q_11 = q_2 >> 2;
	PrintQInt(q_10);
	PrintQInt(q_11);
}
