#include "qfloat.h"
#include "share.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int la_hop_le_Qfloat(char *num, int radix);
bool *lay_Phan_ThapPhan_BD(char *num);
bool *lay_Phan_ThapPhan(int so_mu_int, bool *bits_nguyen,
			bool *bits_thapphan_BD);
bool *lay_Phan_Nguyen(char *num);
int lay_So_Mu_INT(bool *bits_nguyen, bool *bits_thapphan_BD);

void int_to_bi(int x, bool *bits, int bits_size);
bool *int_to_biased(int so_mu_int);
bool *bits_so_cham_dong(bool laSoAm, bool *so_mu_biased, bool *bits_thapphan);

bool *biased_to_bits(bool *biased, int size);
char *chuyen_so_mu_biased(bool *bits);
char *chuyen_nguyen_bits(bool *bits, int so_mu);
void chia_2_str10_thapphan(char *num, int size);
int str10_to_int(char *num);

bool *lay_bits(bool *bits, int from, int to);
int sosanh_bits(bool *a_bits, bool *b_bits, int size);
char *bit_to_str10_thapphan(bool *bits_thapphan, int size_bits);
bool *gan_bits_qfloat(bool *thapphan, bool *somu_biased, bool sign);
bool *str10_to_bit_thapphan(char *num, int size_bits);
bool bits_bang_0(bool *bits, int size);
void float_to_bi_thapphan(float x, bool *bits, int bit_size);
bool Qfloat_bang_0(Qfloat x);

bool *lay_Phan_Nguyen(char *num)
{
	// Kiem tra xem ki tu dau tien co phai la dau + or - hay khong?
	int stop_point = 0;
	if (num[stop_point] == '+' || num[stop_point] == '-') {
		++stop_point;
	}
	// Tach dau cham phan nguyen truoc dau '.'
	char *char_nguyen = strtok(num + stop_point, ".");

	// Khoi tao mang bits cho phan nguyen
	bool *bits_phan_nguyen = (bool *)malloc(sizeof(bool) * Qfloat_MAX_STR2);
	for (int i = 0; i < Qfloat_MAX_STR2; ++i) {
		bits_phan_nguyen[i] = 0;
	}

	// chuyen phan nguyen tu char -> bit
	for (int i = Qfloat_MAX_STR2 - 1; i >= stop_point; --i) {
		if ((char_nguyen[strlen(char_nguyen) - 1] - '0') % 2 == 0) {
			bits_phan_nguyen[i] = 0;
		} else {
			bits_phan_nguyen[i] = 1;
		}
		chia_2_str10(char_nguyen);
	}
	return bits_phan_nguyen;
}

// Ham lay phan thap phan ban dau
bool *lay_Phan_ThapPhan_BD(char *num)
{

	// Tach dau cham phan thap phan sau dau '.'
	char *char_thapphan = strtok(num, ".");
	// Lay phan con lai sau dau "."
	char_thapphan = strtok(NULL, ".");
	// Khoi tao mang bits cho phan thap phan
	bool *bits_phan_thapphan =
	    (bool *)malloc(sizeof(bool) * Qfloat_MAX_STR2);

	// Kiem tra xem co chu so sau dau "." hay khong?
	if (char_thapphan == NULL) {
		for (int i = 0; i < Qfloat_MAX_STR2; ++i) {
			bits_phan_thapphan[i] = 0;
		}
		return bits_phan_thapphan;
	}
	// chuyen phan thap phan tu char -> bit
	int d;
	for (int i = 0; i < Qfloat_MAX_STR2; ++i) {

		d = 0;

		for (int j = strlen(char_thapphan) - 1; j >= 0; --j) {
			int multi = (char_thapphan[j] - '0') * 2 + d;
			char_thapphan[j] = multi % 10 + '0';
			d = multi / 10;
		}
		bits_phan_thapphan[i] = d;
	}
	return bits_phan_thapphan;
}

// Kiem tra Qfloat hop le
// -1: khong hop le, 0: co 0 dau cham dot, 1: co 1 dau cham dot
int la_hop_le_Qfloat(char *num, int radix)
{
	if (!num || strlen(num) <= 0)
		return -1;

	int i = 0;
	if (radix == 10) {
		if (num[i] == '+' || num[i] == '-')
			++i;
	}

	int count_dot = 0;
	for (; num[i] != '\0'; ++i) {
		if (num[i] == '.')
			++count_dot;
		else if (!la_chu_so(num[i], radix) || count_dot > 1)
			return -1;
	}
	if (count_dot == 0) {
		return 0;
	}
	return 1;
}

int lay_So_Mu_INT(bool *bits_nguyen, bool *bits_thapphan_BD)
{
	// Tim vi tri 1. trong bits_nguyen
	int dot = 0;
	for (; bits_nguyen[dot] != 1 && dot < Qfloat_MAX_STR2; ++dot) {
	}
	// Xet bits_nguyen co toan = 0 hay khong?
	if (dot == Qfloat_MAX_STR2) {
		dot = 1;
		// Tim vi tri 1. trong bits_thaphan. Qfloat_MAX_STR2 -2 vì khong
		// the chua qua -Qfloat_MAX_STR2-1
		for (int j = 0;
		     bits_thapphan_BD[j] != 1 && j < Qfloat_MAX_STR2 - 2;
		     ++dot, ++j) {
		}
		return -dot;
	}

	return Qfloat_MAX_STR2 - dot - 1;
}

// Lay ra phan thap phan that su
bool *lay_Phan_ThapPhan(int so_mu_int, bool *bits_nguyen,
			bool *bits_thapphan_BD)
{
	bool *bits_thaphan =
	    (bool *)malloc(sizeof(bool *) * Qfloat_Bits_ThapPhan);

	for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
		bits_thaphan[i] = 0;
	}

	if (so_mu_int == 0) {
		for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
			bits_thaphan[i] = bits_thapphan_BD[i];
		}
	} else if (so_mu_int > 0) {
		int i = 0;
		int j = Qfloat_MAX_STR2 - so_mu_int;
		// Copy bits sau 1. trong phan nguyen
		for (; j < Qfloat_MAX_STR2 && i < Qfloat_Bits_ThapPhan; ++j) {
			bits_thaphan[i++] = bits_nguyen[j];
		}
		// Copy bits trong phan thap phan ban dau. Phan du cua phan thap
		// phan ban dau sau do khong su dung
		j = 0;
		for (; i < Qfloat_Bits_ThapPhan; ++i) {
			bits_thaphan[i] = bits_thapphan_BD[j++];
		}
	} else {
		// so_mu_int < 0

		int i = 0;
		int j = -so_mu_int;

		// Copy bits sau 1. trong phan thap phan
		for (; j < Qfloat_MAX_STR2 && i < Qfloat_Bits_ThapPhan; ++j) {
			bits_thaphan[i++] = bits_thapphan_BD[j];
		}
	}
	return bits_thaphan;
}

// Chuyen int qua binary
void int_to_bi(int x, bool *bits, int bits_size)
{
	for (int i = bits_size - 1; i >= 0; --i) {
		bits[i] = x & 1;
		x >>= 1;
	}
}

// chuyen int qua biased
bool *int_to_biased(int so_mu_int)
{
	// Dua so_mu_int ve binary
	bool *int_bi = (bool *)malloc(sizeof(bool) * Qfloat_Bits_Mu);
	int_to_bi(so_mu_int, int_bi, Qfloat_Bits_Mu);
	// Thiet lap so 0 biased
	bool *biased_0 = (bool *)malloc(sizeof(bool) * Qfloat_Bits_Mu);
	biased_0[0] = 0;
	for (int i = 1; i < Qfloat_Bits_Mu; ++i) {
		biased_0[i] = 1;
	}

	bool *biased = cong_bits(int_bi, biased_0, Qfloat_Bits_Mu);

	free(biased_0);
	free(int_bi);
	return biased;
}

// Thiet lap day bits so cham dong
bool *bits_so_cham_dong(bool laSoAm, bool *so_mu_biased, bool *bits_thapphan)
{
	bool *bits_qfloat = (bool *)malloc(sizeof(bool) * Qfloat_bits_size);
	// gan so am
	bits_qfloat[Qfloat_bits_size - 1] = laSoAm;
	// Gan phan thap phan
	int i = 0;
	for (; i < Qfloat_Bits_ThapPhan; ++i) {
		bits_qfloat[i] = bits_thapphan[i];
	}
	// gan phan mu biased
	int j = 0;
	for (; i < Qfloat_bits_size - 1; ++i) {
		bits_qfloat[i] = so_mu_biased[j++];
	}
	return bits_qfloat;
}

// Chuyen chuoi float 10 sang bits
bool *float_str10_to_bit(char *num)
{
	int dot = la_hop_le_Qfloat(num, 10);
	if (dot == -1) {
		printf("Input khong hop le\n");
		return NULL;
	}

	bool laSoAm = 0;
	if (num[0] == '-') {
		laSoAm = 1;
	}

	char temp[Qfloat_MAX_STR10];
	strcpy(temp, num);

	// Phan nguyen cua so nguyen nhap vao
	bool *bits_nguyen = lay_Phan_Nguyen(temp);

	strcpy(temp, num);
	// Phan thap phan cua so nguyen nhap vao
	bool *bits_thapphan_BD = lay_Phan_ThapPhan_BD(temp);

	int so_mu_int = lay_So_Mu_INT(bits_nguyen, bits_thapphan_BD);
	// Phan thap phan that su dang chuan
	bool *bits_thapphan =
	    lay_Phan_ThapPhan(so_mu_int, bits_nguyen, bits_thapphan_BD);

	free(bits_nguyen);
	free(bits_thapphan_BD);

	bool *so_mu_biased = int_to_biased(so_mu_int);

	bool *qfloat_bits =
	    bits_so_cham_dong(laSoAm, so_mu_biased, bits_thapphan);

	free(so_mu_biased);
	free(bits_thapphan);

	return qfloat_bits;
}

Qfloat BinToDec_float(bool *bits)
{
	Qfloat x;
	for (int i = 0, j = 0; i < 4; ++i, j += 32) {
		x.block[i] = seq_to_int(bits, j, j + 31);
	}
	return x;
}

void ScanQfloat(Qfloat &x)
{
	char num[Qfloat_MAX_STR10];

	printf("Nhap so thuc lon: ");
	scanf("%s", num);

	bool *qfloat_bits = float_str10_to_bit(num);

	if (qfloat_bits != NULL)
		x = BinToDec_float(qfloat_bits);

	free(qfloat_bits);
}

// Chuyen doi theo YEUCAU, mac dinh bits la 128
bool *DecToBin_float(Qfloat q)
{
	bool *bits = (bool *)malloc(sizeof(bool) * Qfloat_bits_size);
	for (int i = 0, y = 0; i < 4; ++i, y += 32) {
		int_to_seq(q.block[i], bits, y, y + 31);
	}
	return bits;
}

// Chuyen so biased sang bits so bu 2
bool *biased_to_bits(bool *biased, int size)
{
	// Thiet lap so 0 biased
	bool *biased_0 = (bool *)malloc(sizeof(bool) * size);
	biased_0[0] = 0;
	for (int i = 1; i < size; ++i) {
		biased_0[i] = 1;
	}

	bool *bits = tru_bits(biased, biased_0, size);

	free(biased_0);
	return bits;
}

// Chuyen so mu tu biased sang chuoi co so 10
char *chuyen_so_mu_tu_biased(bool *bits)
{
	// So so mu biased
	bool *so_mu_biased = (bool *)malloc(sizeof(bool) * Qfloat_Bits_Mu);
	int j = 0;
	for (int i = Qfloat_Bits_ThapPhan;
	     i < Qfloat_Bits_ThapPhan + Qfloat_Bits_Mu; ++i) {
		so_mu_biased[j++] = bits[i];
	}

	// chuyen so mu biased thanh so mu bits so bu 2
	bool *so_mu_bits = biased_to_bits(so_mu_biased, Qfloat_Bits_Mu);

	// chuyen so mu bits so bu 2 thanh char* co so 10
	char *so_mu_char = bit_to_str10(so_mu_bits, Qfloat_Bits_Mu);

	free(so_mu_biased);
	free(so_mu_bits);

	return so_mu_char;
}

// Chuyen thap phan bits sang chuoi co so 10
char *chuyen_thaphan_bits(bool *bits, int so_mu)
{

	int max_size = Qfloat_MAX_STR10;
	// Tao chuoi thap phan str 10
	char *thapphan_str10 = (char *)malloc(sizeof(char) * max_size + 1);
	thapphan_str10[max_size] = '\0';

	for (int i = 0; i < max_size; ++i) {
		thapphan_str10[i] = '0';
	}

	// so_mu > Qfloat_Bits_ThapPhan thi phan thap phan = 0
	if (so_mu > Qfloat_Bits_ThapPhan) {
		return thapphan_str10;
	}

	// Khoi tao phan thap phan
	bool *bits_thapphan =
	    (bool *)malloc(sizeof(bool) * Qfloat_Bits_ThapPhan);

	for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
		bits_thapphan[i] = 0;
	}

	// Kiem tra so_mu de dich bits
	if (so_mu >= 0 && so_mu < Qfloat_Bits_ThapPhan) {
		int j = 0;
		for (int i = so_mu; i < Qfloat_Bits_ThapPhan; ++i) {
			bits_thapphan[j++] = bits[i];
		}
		// Lay bits thap phan sau khi dich qua trai so_mu bits
	} else if (so_mu < 0 && so_mu > -Qfloat_Bits_ThapPhan + 1) {
		int i = -so_mu;
		bits_thapphan[i - 1] = 1;
		int j = 0;
		for (; i < Qfloat_Bits_ThapPhan; ++i) {
			bits_thapphan[i] = bits[j++];
		}
		so_mu = 0;
	} else {
		// so_mu <= -Qfloat_Bits_ThapPhan + 1
		int i = 1;
		// dich sang trai 1 bit
		bits_thapphan[i - 1] = 1;
		for (; i < Qfloat_Bits_ThapPhan; ++i) {
			bits_thapphan[i] = bits[i - 1];
		}
		so_mu += 1;
	}

	for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
		if (bits_thapphan[i] == 1) {
			// lay 2^(-i)
			char *temp =
			    (char *)malloc(sizeof(char) * max_size + 1);
			temp[max_size] = '\0';

			temp[0] = '5';
			for (int j = 1; j < max_size; ++j) {
				temp[j] = '0';
			}

			for (int j = 0; j < i; ++j) {
				chia_2_str10_thapphan(temp, max_size);
			}
			cong_str10(thapphan_str10, temp);
			free(temp);
		}
	}
	free(bits_thapphan);

	if (so_mu < 0) {
		int n = -so_mu;
		for (int i = 0; i <= n; ++i) {
			chia_2_str10_thapphan(thapphan_str10, max_size);
		}
	}
	// printf("\n%s\n",thapphan_str10);
	// Rut gon chuoi so
	// Tim vi tri cuoi cung ben phai cung khac khong
	int cungcuoi = max_size - 1;
	for (; thapphan_str10[cungcuoi] == '0'; --cungcuoi) {
	}

	if (cungcuoi >= 0) {
		thapphan_str10 = (char *)realloc(thapphan_str10,
						 sizeof(char) * cungcuoi + 2);
		thapphan_str10[cungcuoi + 1] = '\0';
	} else {
		thapphan_str10 =
		    (char *)realloc(thapphan_str10, sizeof(char) * 2);
		thapphan_str10[1] = '\0';
		thapphan_str10[0] = '0';
	}
	return thapphan_str10;
}

// Chuyen phan nguyen bits sang chuoi co so 10
char *chuyen_nguyen_bits(bool *bits, int so_mu)
{
	// Khoi tao phan nguyen
	bool *bits_nguyen =
	    (bool *)malloc(sizeof(bool) * Qfloat_Bits_ThapPhan + 1);
	for (int i = 0; i < Qfloat_Bits_ThapPhan + 1; ++i) {
		bits_nguyen[i] = 0;
	}

	// Kiem tra so_mu de dich bits
	// so_mu < 0 ==> phan nguyen = 0
	if (so_mu > 0) {
		int i = Qfloat_Bits_ThapPhan + 1 - so_mu;
		if (so_mu >= Qfloat_Bits_ThapPhan) {
			i = 2;
		}
		bits_nguyen[i - 1] = 1;
		// Lay bits nguyen sau khi dich qua trai so_mu bits
		int j = 0;
		for (; i < Qfloat_Bits_ThapPhan + 1; ++i) {
			bits_nguyen[i] = bits[j++];
		}

	} else if (so_mu == 0) {
		// Phan_nguyen = 1
		bits_nguyen[Qfloat_Bits_ThapPhan] = 1;
	}

	// Tao chuoi thap phan str 10
	char *nguyen_str10 =
	    bit_to_str10(bits_nguyen, Qfloat_Bits_ThapPhan + 1);
	free(bits_nguyen);
	int len_nguyen_str10 = strlen(nguyen_str10);

	// tao size lon nhat cua phan nguyen
	char *temp = (char *)malloc(Qfloat_MAX_STR10 * sizeof(char) + 1);
	temp[Qfloat_MAX_STR10] = '\0';

	for (int i = 0; i < Qfloat_MAX_STR10 - len_nguyen_str10; ++i) {
		temp[i] = '0';
	}
	// copy nguyen_str10 vao cuoi chuoi temp
	int j = 0;
	for (int i = Qfloat_MAX_STR10 - len_nguyen_str10; i < Qfloat_MAX_STR10;
	     ++i) {
		temp[i] = nguyen_str10[j++];
	}
	// Xoa bien nguyen_str10 cu
	free(nguyen_str10);

	// phan nguyen_str10 * 2^(so_mu - Qfloat_Bits_ThapPhan + 1)
	if (so_mu >= Qfloat_Bits_ThapPhan) {

		for (int i = 0; i < so_mu - Qfloat_Bits_ThapPhan + 1; ++i) {
			nhan_2_str10(temp);
		}
	}

	// Rut gon chuoi so
	// Tim vi tri dau tien khac khong
	int dautien = 0;
	for (; temp[dautien] == '0'; ++dautien) {
	}

	int nguyen_size =
	    (dautien >= Qfloat_MAX_STR10) ? 1 : (Qfloat_MAX_STR10 - dautien);

	nguyen_str10 = (char *)malloc(sizeof(char) * (nguyen_size + 1));
	for (int i = 0; i < nguyen_size; ++i) {
		nguyen_str10[i] = '0';
	}
	nguyen_str10[nguyen_size] = '\0';

	// copy nhung so co nghia vao chuoi nguyen_str10
	for (int i = dautien; i < Qfloat_MAX_STR10; ++i) {
		nguyen_str10[i - dautien] = temp[i];
	}

	free(temp);
	return nguyen_str10;
}

void chia_2_str10_thapphan(char *num, int size)
{
	char *thuong = (char *)malloc(sizeof(char) * size + 1);
	thuong[size] = '\0';

	for (int k = 0; k < size; ++k) {
		thuong[k] = '0';
	}

	// index mang num
	int i = 0;
	if (num[i] == '-' || num[i] == '+')
		++i;

	// Tim so chu so != 0 dau tien trong day thap phan num
	for (; num[i] == '0'; ++i) {
	}

	int temp = num[i] - '0';
	// index mang thuong
	int j = i;

	if (num[i] - '0' < 2) {
		if (num[i + 1] == '\0') {
			strcpy(num, "0");
			free(thuong);
			return;
		}
		temp = temp * 10 + num[i + 1] - '0';
		++i;
		++j;
	}

	for (; num[i] != '\0'; ++i) {
		thuong[j++] = temp / 2 + '0';
		temp = (temp % 2) * 10 + num[i + 1] - '0';
	}
	// So du con lai
	if (temp == 1 && j < size) {
		thuong[j++] = '5';
	}
	strcpy(num, thuong);
	free(thuong);
}

// Chuyen chuoi co so 10 sang so nguyen co so 10
int str10_to_int(char *num)
{
	int laSoAm = 0;
	if (num[0] == '-') {
		++laSoAm;
	}
	int int_num = 0;
	for (int i = laSoAm; num[i] != '\0'; ++i) {
		int_num = int_num * 10 + (num[i] - '0');
	}
	return laSoAm ? -int_num : int_num;
}

// Chuyen Qfloat dang bit sang chuoi co so 10
char *Qfloat_bit_to_str10(bool *bits)
{
	// Kiem tra so 0
	bool laSo0 = true;
	for (int i = 0; i < Qfloat_bits_size - 1; ++i) {
		if (bits[i] == 1)
			laSo0 = false;
	}
	if (laSo0) {
		char *so0 = (char *)malloc(sizeof(char) * 2);
		so0[0] = '0';
		so0[1] = '\0';
		return so0;
	}

	// Kiem tra so am
	int laSoAm = 0;
	if (bits[Qfloat_bits_size - 1] == 1) {
		laSoAm = 1;
	}
	// chuyen so mu thanh char* co so 10
	char *so_mu = chuyen_so_mu_tu_biased(bits);

	int so_mu_int = str10_to_int(so_mu);

	// chuyen so nguyen thanh char* co so 10
	char *phan_nguyen = chuyen_nguyen_bits(bits, so_mu_int);
	// chuyen so thap phan thanh char* co so 10
	char *thap_phan = chuyen_thaphan_bits(bits, so_mu_int);

	bool so_0_thapphan = true;
	for (int i = 0; thap_phan[i] != '\0'; ++i) {
		if (thap_phan[i] != '0') {
			so_0_thapphan = false;
		}
	}

	char *new_num = (char *)malloc(sizeof(char) * Qfloat_MAX_STR10 + 1);
	new_num[Qfloat_MAX_STR10] = '\0';

	if (laSoAm)
		new_num[0] = '-';
	// index mang new_num
	int index = laSoAm;
	// copy phan_nguyen vao mang new_num
	for (int i = 0; phan_nguyen[i] != '\0'; ++i) {
		new_num[index++] = phan_nguyen[i];
	}
	// copy thap_phan vao mang new_num
	if (!so_0_thapphan) {
		new_num[index++] = '.';
		for (int i = 0; thap_phan[i] != '\0'; ++i) {
			new_num[index++] = thap_phan[i];
		}
	}
	new_num[index] = '\0';

	free(so_mu);
	free(phan_nguyen);
	free(thap_phan);
	return new_num;
}

void PrintQfloat(Qfloat q)
{

	printf("Xuat so thuc lon: ");
	bool *bits = DecToBin_float(q);
	char *str10 = Qfloat_bit_to_str10(bits);
	printf("%s\n", str10);
	free(bits);
	free(str10);
}

bool Qfloat_bang_0(Qfloat x)
{
	bool x_0 = true;
	for (int i = 0; i < Qfloat_block_size - 1; ++i) {
		if (x.block[i] != 0) {
			x_0 = false;
		}
	}
	if (x_0 && (x.block[Qfloat_block_size - 1] == 0 ||
		    x.block[Qfloat_block_size - 1] == 1)) {
		return true;
	}
	return false;
}

// Nhan 2 so Qfloat, z = x*y;
Qfloat operator*(Qfloat x, Qfloat y)
{
	Qfloat z;
	for (int i = 0; i < Qfloat_block_size; ++i) {
		z.block[i] = 0;
	}

	// Xet x == 0?
	if (Qfloat_bang_0(x) || Qfloat_bang_0(y)) {
		return z;
	}
	// Lay thong Qfloat x
	bool *x_bits = DecToBin_float(x);
	char *mu_x_char = chuyen_so_mu_tu_biased(x_bits);
	int mu_x = str10_to_int(mu_x_char);
	free(mu_x_char);

	// Lay thong Qfloat y
	bool *y_bits = DecToBin_float(y);
	char *mu_y_char = chuyen_so_mu_tu_biased(y_bits);
	int mu_y = str10_to_int(mu_y_char);
	free(mu_y_char);

	int mu_z = mu_x + mu_y + 2;

	// Vuot qua gioi han luu tru
	if (mu_z < -Qfloat_MAX_STR2 + 1 || mu_z > Qfloat_MAX_STR2 - 1) {
		free(x_bits);
		free(y_bits);
		printf("Khong the bieu dien phep nhan");
		return z;
	}

	char *x_thapphan = chuyen_thaphan_bits(x_bits, -1);
	char *y_thapphan = chuyen_thaphan_bits(y_bits, -1);

	// chuyen x_thapphan sang day bit
	int max_size = Qfloat_MAX_STR2;
	bool *x_thapphan_bits = str10_to_bit(x_thapphan, max_size);
	free(x_thapphan);

	// chuyen y_thapphan sang day bit
	bool *y_thapphan_bits = str10_to_bit(y_thapphan, max_size);
	free(y_thapphan);

	bool *dinh_tri = nhan_bits(x_thapphan_bits, y_thapphan_bits, max_size);

	// chuyen dinh tri thanh char
	char *dinh_tri_char = bit_to_str10(dinh_tri, max_size);
	free(dinh_tri);

	dinh_tri = (bool *)malloc(sizeof(bool) * max_size);
	// chuyen phan thap phan char -> bit
	int d;
	for (int i = 0; i < max_size; ++i) {
		d = 0;
		for (int j = strlen(dinh_tri_char) - 1; j >= 0; --j) {
			int multi = (dinh_tri_char[j] - '0') * 2 + d;
			dinh_tri_char[j] = multi % 10 + '0';
			d = multi / 10;
		}
		dinh_tri[i] = d;
	}

	free(dinh_tri_char);
	free(x_thapphan_bits);
	free(y_thapphan_bits);

	// Tim bit 1 ben trai cung phan dinh tri
	int dautien = 0;
	for (; dautien < Qfloat_Bits_ThapPhan && dinh_tri[dautien] == 0;
	     ++dautien) {
	}
	mu_z -= (dautien + 1);
	bool *mu_z_biased = int_to_biased(mu_z);

	bool *Qfloat_bits = (bool *)malloc(sizeof(bool) * Qfloat_bits_size);
	// gan dau + -
	Qfloat_bits[Qfloat_bits_size - 1] =
	    x_bits[Qfloat_bits_size - 1] == y_bits[Qfloat_bits_size - 1] ? 0
									 : 1;
	// gan so mu
	int j = 0;
	for (int i = Qfloat_Bits_ThapPhan;
	     i < Qfloat_Bits_ThapPhan + Qfloat_Bits_Mu; ++i) {
		Qfloat_bits[i] = mu_z_biased[j++];
	}

	// gan phan thap phan
	int i = 0;
	for (; i < Qfloat_Bits_ThapPhan && dautien < Qfloat_Bits_ThapPhan;
	     ++i) {
		Qfloat_bits[i] = dinh_tri[++dautien];
	}
	for (; i < Qfloat_Bits_ThapPhan; ++i) {
		Qfloat_bits[i] = 0;
	}

	free(dinh_tri);
	free(x_bits);
	free(y_bits);
	free(mu_z_biased);

	z = BinToDec_float(Qfloat_bits);
	free(Qfloat_bits);
	return z;
}

// chuyen thap phan float qua binary 0<x<1
void float_to_bi_thapphan(float x, bool *bits, int bit_size)
{
	float t = x;
	for (int i = 0; i < bit_size; ++i) {
		t *= 2;
		bits[i] = (int)t % 2;
		t = t - bits[i];
	}
}

Qfloat operator/(Qfloat x, Qfloat y)
{
	Qfloat z;
	for (int i = 0; i < Qfloat_block_size; ++i) {
		z.block[i] = 0;
	}

	// Xet x == 0?
	if (Qfloat_bang_0(x) || Qfloat_bang_0(y)) {
		return z;
	}
	// Lay thong Qfloat x
	bool *x_bits = DecToBin_float(x);
	char *mu_x_char = chuyen_so_mu_tu_biased(x_bits);
	int mu_x = str10_to_int(mu_x_char);
	free(mu_x_char);

	// Lay thong Qfloat y
	bool *y_bits = DecToBin_float(y);
	char *mu_y_char = chuyen_so_mu_tu_biased(y_bits);
	int mu_y = str10_to_int(mu_y_char);
	free(mu_y_char);

	int mu_z = mu_x - mu_y;

	// Vuot qua gioi han luu tru
	if (mu_z < -Qfloat_MAX_STR2 + 1 || mu_z > Qfloat_MAX_STR2 - 1) {
		free(x_bits);
		free(y_bits);
		printf("Khong the bieu dien phep chia");
		return z;
	}

	double x_dinhtri = 1;
	double y_dinhtri = 1;
	for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
		if (x_bits[i] == 1 || y_bits[i] == 1) {
			double t = 1;
			for (int j = 0; j <= i; ++j) {
				t /= 2;
			}
			if (x_bits[i] == 1) {
				x_dinhtri += t;
			}
			if (y_bits[i] == 1) {
				y_dinhtri += t;
			}
		}
	}

	double z_dinhtri = x_dinhtri / y_dinhtri;
	int one = 0;
	if (z_dinhtri > 1) {
		one = 1;
		z_dinhtri -= 1;
	}

	bool *dinh_tri = (bool *)malloc(sizeof(bool) * Qfloat_Bits_ThapPhan);
	float_to_bi_thapphan(z_dinhtri, dinh_tri, Qfloat_Bits_ThapPhan);

	// Tim bit 1 ben trai cung phan dinh tri
	int dautien = 0;
	if (one != 1) {
		for (; dautien < Qfloat_Bits_ThapPhan && dinh_tri[dautien] == 0;
		     ++dautien) {
		}
		mu_z -= (dautien + 1);
	} else {
		dautien = -1;
	}
	bool *mu_z_biased = int_to_biased(mu_z);

	bool *Qfloat_bits = (bool *)malloc(sizeof(bool) * Qfloat_bits_size);
	// gan dau + -
	Qfloat_bits[Qfloat_bits_size - 1] =
	    x_bits[Qfloat_bits_size - 1] == y_bits[Qfloat_bits_size - 1] ? 0
									 : 1;
	// gan so mu
	int j = 0;
	for (int i = Qfloat_Bits_ThapPhan;
	     i < Qfloat_Bits_ThapPhan + Qfloat_Bits_Mu; ++i) {
		Qfloat_bits[i] = mu_z_biased[j++];
	}

	// gan phan thap phan
	int i = 0;
	for (; i < Qfloat_Bits_ThapPhan && dautien < Qfloat_Bits_ThapPhan;
	     ++i) {
		Qfloat_bits[i] = dinh_tri[++dautien];
	}
	for (; i < Qfloat_Bits_ThapPhan; ++i) {
		Qfloat_bits[i] = 0;
	}

	free(dinh_tri);
	free(x_bits);
	free(y_bits);
	free(mu_z_biased);

	z = BinToDec_float(Qfloat_bits);
	free(Qfloat_bits);
	return z;
}

bool *lay_bits(bool *bits, int from, int to)
{
	bool *t = (bool *)malloc(sizeof(bool) * (to - from + 1));
	int j = 0;
	for (int i = from; i <= to; ++i) {
		t[j++] = bits[i];
	}
	return t;
}

// so sanh hai day bits a va b khong dau co kich thuoc bang nhau,
// a < b return -1, a = b return 0 ,a > b return 1
int sosanh_bits(bool *a_bits, bool *b_bits, int size)
{
	for (int i = 0; i < size; ++i) {
		if (a_bits[i] ^ b_bits[i]) {
			if (a_bits[i] == 1) {
				return 1;
			} else {
				return -1;
			}
		}
	}
	return 0;
}

bool bits_bang_0(bool *bits, int size)
{
	for (int i = 0; i < size; ++i) {
		if (bits[i] == 1) {
			return false;
		}
	}
	return true;
}

// chuyen bit thap phan thanh str10, phan sau dau .
char *bit_to_str10_thapphan(bool *bits_thapphan, int size_bits)
{

	int max_size = (int)((size_bits * 3) / 10) + 1;

	char *thapphan_str10 = (char *)malloc(sizeof(char) * max_size + 1);
	for (int i = 0; i < max_size; ++i) {
		thapphan_str10[i] = '0';
	}
	thapphan_str10[max_size] = '\0';

	for (int i = 0; i < Qfloat_Bits_ThapPhan; ++i) {
		if (bits_thapphan[i] == 1) {

			// lay 2^(-i)
			char *temp =
			    (char *)malloc(sizeof(char) * max_size + 1);
			temp[max_size] = '\0';

			temp[0] = '5';
			for (int j = 1; j < max_size; ++j) {
				temp[j] = '0';
			}

			for (int j = 0; j < i; ++j) {
				chia_2_str10_thapphan(temp, max_size);
			}
			cong_str10(thapphan_str10, temp);
			free(temp);
		}
	}
	return thapphan_str10;
}

bool *str10_to_bit_thapphan(char *num, int size_bits)
{
	char *tem = (char *)malloc(sizeof(char) * strlen(num) + 1);
	strcpy(tem, num);

	bool *dinh_tri = (bool *)malloc(sizeof(bool) * size_bits);
	// chuyen phan thap phan char -> bit
	int d;
	for (int i = 0; i < size_bits; ++i) {
		d = 0;
		for (int j = strlen(tem) - 1; j >= 0; --j) {
			int multi = (tem[j] - '0') * 2 + d;
			tem[j] = multi % 10 + '0';
			d = multi / 10;
		}
		dinh_tri[i] = d;
	}
	free(tem);
	return dinh_tri;
}

bool *gan_bits_qfloat(bool *thapphan, bool *somu_biased, bool sign)
{
	bool *Qfloat_bits = (bool *)malloc(sizeof(bool) * Qfloat_bits_size);
	// gan dau + -
	Qfloat_bits[Qfloat_bits_size - 1] = sign;
	// gan so mu
	int j = 0;
	for (int i = Qfloat_Bits_ThapPhan;
	     i < Qfloat_Bits_ThapPhan + Qfloat_Bits_Mu; ++i) {
		Qfloat_bits[i] = somu_biased[j++];
	}

	// gan phan thap phan
	int i = 0;
	for (; i < Qfloat_Bits_ThapPhan; ++i) {
		Qfloat_bits[i] = thapphan[i];
	}
	return Qfloat_bits;
}

Qfloat operator+(Qfloat x, Qfloat y)
{
	Qfloat z;
	for (int i = 0; i < Qfloat_block_size; ++i) {
		z.block[i] = 0;
	}
	// Truong hop x = 0
	if (Qfloat_bang_0(x)) {
		for (int i = 0; i < Qfloat_block_size; ++i) {
			z.block[i] = y.block[i];
		}
		return y;
	}
	// Truong hop y = 0
	if (Qfloat_bang_0(y)) {
		for (int i = 0; i < Qfloat_block_size; ++i) {
			z.block[i] = x.block[i];
		}
		return x;
	}

	// lay thong tin Qfloat x
	bool *x_bits = DecToBin_float(x);
	bool *x_mu_biased =
	    lay_bits(x_bits, Qfloat_Bits_ThapPhan, Qfloat_bits_size - 2);
	bool *x_thapphan = lay_bits(x_bits, 0, Qfloat_Bits_ThapPhan - 1);
	// Dich so thap phan qua phai 1 bit de lay so 1 phan nguyen
	dich_phai_1_bit(x_thapphan, Qfloat_Bits_ThapPhan);
	x_thapphan[0] = 1;
	cong_1_bit(x_mu_biased, Qfloat_Bits_Mu);
	// Dich phai x_thapphan 1 bit de co phan du khi thuc hien phep cong
	dich_phai_1_bit(x_thapphan, Qfloat_Bits_ThapPhan);
	x_thapphan[0] = 0;
	cong_1_bit(x_mu_biased, Qfloat_Bits_Mu);

	// lay thong tin Qfloat y
	bool *y_bits = DecToBin_float(y);
	bool *y_mu_biased =
	    lay_bits(y_bits, Qfloat_Bits_ThapPhan, Qfloat_bits_size - 2);
	bool *y_thapphan = lay_bits(y_bits, 0, Qfloat_Bits_ThapPhan - 1);
	// Dich so thap phan qua phai 1 bit de bit 1 phan nguyen
	dich_phai_1_bit(y_thapphan, Qfloat_Bits_ThapPhan);
	y_thapphan[0] = 1;
	cong_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	// Dich phai y_thapphan 1 bit de co phan du khi thuc hien phep cong
	dich_phai_1_bit(y_thapphan, Qfloat_Bits_ThapPhan);
	y_thapphan[0] = 0;
	cong_1_bit(y_mu_biased, Qfloat_Bits_Mu);

	// Xet x < y hay khong? -1 la x < y, 1 la x > y, 0 la x = y
	int tem = sosanh_bits(x_mu_biased, y_mu_biased, Qfloat_Bits_Mu);
	// Tim so lon hon
	bool x_lonhon_y = true;
	if (tem == -1 ||
	    (tem == 0 &&
	     sosanh_bits(x_thapphan, y_thapphan, Qfloat_Bits_ThapPhan) == -1)) {
		x_lonhon_y = 0;
	}

	// Truong hop so mu khac nhau
	// Tang so mu cua so nho hon
	while (sosanh_bits(x_mu_biased, y_mu_biased, Qfloat_Bits_Mu) != 0) {
		if (tem == -1) {
			cong_1_bit(x_mu_biased, Qfloat_Bits_Mu);
			dich_phai_1_bit(x_thapphan, Qfloat_Bits_ThapPhan);
			x_thapphan[0] = 0;
			if (bits_bang_0(x_thapphan, Qfloat_Bits_ThapPhan)) {
				for (int i = 0; i < Qfloat_block_size; ++i) {
					z.block[i] = y.block[i];
				}
				free(x_bits);
				free(x_thapphan);
				free(x_mu_biased);
				free(y_bits);
				free(y_thapphan);
				free(y_mu_biased);
				return y;
			}
		} else if (tem == 1) {
			cong_1_bit(y_mu_biased, Qfloat_Bits_Mu);
			dich_phai_1_bit(y_thapphan, Qfloat_Bits_ThapPhan);
			y_thapphan[0] = 0;
			if (bits_bang_0(y_thapphan, Qfloat_Bits_ThapPhan)) {
				for (int i = 0; i < Qfloat_block_size; ++i) {
					z.block[i] = x.block[i];
				}
				free(x_bits);
				free(x_thapphan);
				free(x_mu_biased);
				free(y_bits);
				free(y_thapphan);
				free(y_mu_biased);
				return x;
			}
		}
	}

	// Truong hop so mu bang nhau
	// Cong 2 thap phan co dau
	// Chuyen x_thapphan thanh bit nhu phan nguyen de thuc hien phep cong
	char *x_thapphan_char =
	    bit_to_str10_thapphan(x_thapphan, Qfloat_Bits_ThapPhan);
	// Chuyen y_thapphan thanh bit nhu phan nguyen de thuc hien phep cong
	char *y_thapphan_char =
	    bit_to_str10_thapphan(y_thapphan, Qfloat_Bits_ThapPhan);

	char *z_thapphan_char;
	// Xet x va y cung dau
	if (x_bits[Qfloat_bits_size - 1] == y_bits[Qfloat_bits_size - 1]) {
		z_thapphan_char =
		    cong_str10_unsigned(x_thapphan_char, y_thapphan_char);
	} else if (x_lonhon_y) {
		// x va y khac dau
		z_thapphan_char =
		    tru_str10_unsigned(x_thapphan_char, y_thapphan_char);
	} else {
		// x va y khac dau
		z_thapphan_char =
		    tru_str10_unsigned(y_thapphan_char, x_thapphan_char);
	}

	// Neu dinh tri = 0
	if (sosanh_bits(y_thapphan, x_thapphan, Qfloat_Bits_ThapPhan) == 0 &&
	    y_bits[Qfloat_bits_size - 1] != x_bits[Qfloat_bits_size - 1]) {
		free(x_bits);
		free(x_thapphan);
		free(x_mu_biased);
		free(x_thapphan_char);
		free(y_bits);
		free(y_thapphan);
		free(y_mu_biased);
		free(y_thapphan_char);
		free(z_thapphan_char);
		return z;
	}

	// Lay so mu y lam so so mu z. (vi so_mu y = so_mu x)
	// Neu dinh tri(thap phan) vuot qua gioi han
	tru_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	if (z_thapphan_char[0] == '0') {
		tru_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	}
	bool overflow[15] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	// Kiem tra so mu co overflow
	if (sosanh_bits(y_mu_biased, overflow, Qfloat_Bits_Mu) == 1) {
		printf("Vuot qua gioi han tren");
		free(x_bits);
		free(x_thapphan);
		free(x_mu_biased);
		free(x_thapphan_char);
		free(y_bits);
		free(y_thapphan);
		free(y_mu_biased);
		free(y_thapphan_char);
		free(z_thapphan_char);
		return z;
	}
	// Tra lai so mu truoc khi kiem tra
	cong_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	if (z_thapphan_char[0] == '0') {
		cong_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	}
	// Chuan hoa lai phan dinh tri
	bool *z_bits_thapphan =
	    str10_to_bit_thapphan(z_thapphan_char, Qfloat_Bits_ThapPhan);
	// Tim bit 1 ben trai cung phan dinh tri
	int dautien = 0;
	for (; dautien < Qfloat_Bits_ThapPhan && z_bits_thapphan[dautien] == 0;
	     ++dautien)
		;

	int j = dautien;
	int k = 0;

	for (; k < Qfloat_Bits_ThapPhan && dautien < Qfloat_Bits_ThapPhan;
	     ++k) {
		z_bits_thapphan[k] = z_bits_thapphan[++j];
	}
	for (; k < Qfloat_Bits_ThapPhan; ++k) {
		z_bits_thapphan[k] = 0;
	}
	// Dich dau . qua sau vi tri bit 1 ben trai cung dau tien
	// giam so mu
	for (int i = 0; i <= dautien; ++i) {
		tru_1_bit(y_mu_biased, Qfloat_Bits_Mu);
	}
	// Kiem tra so mu co underflow
	bool underflow[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	if (sosanh_bits(y_mu_biased, underflow, Qfloat_Bits_Mu) == -1) {
		printf("Vuot qua gioi han duoi");
		free(x_bits);
		free(x_thapphan);
		free(x_mu_biased);
		free(x_thapphan_char);
		free(y_bits);
		free(y_thapphan);
		free(y_mu_biased);
		free(y_thapphan_char);
		free(z_bits_thapphan);
		free(z_thapphan_char);
		return z;
	}
	// Lay dau cua phep cong
	bool sign;
	// tem = sosanh_bits(x_thapphan, y_thapphan, Qfloat_Bits_ThapPhan);
	if (x_lonhon_y) {
		sign = x_bits[Qfloat_bits_size - 1];
	} else {
		sign = y_bits[Qfloat_bits_size - 1];
	}

	bool *Qfloat_bits = gan_bits_qfloat(z_bits_thapphan, y_mu_biased, sign);
	z = BinToDec_float(Qfloat_bits);

	free(x_bits);
	free(x_thapphan);
	free(x_mu_biased);
	free(x_thapphan_char);
	free(y_bits);
	free(y_thapphan);
	free(y_mu_biased);
	free(y_thapphan_char);
	free(z_bits_thapphan);
	free(z_thapphan_char);
	free(Qfloat_bits);
	return z;
}

Qfloat operator-(Qfloat x, Qfloat y)
{
	Qfloat tem;
	bool *y_bits = DecToBin_float(y);
	y_bits[Qfloat_bits_size - 1] = 1 - y_bits[Qfloat_bits_size - 1];
	tem = BinToDec_float(y_bits);
	free(y_bits);
	return x + tem;
}
void testScanQfloat()
{
	printf("Test input output\n");
	Qfloat q;
	ScanQfloat(q);
	PrintQfloat(q);
}

void testCongTruNhanChiaQfloat()
{
	printf("Test input output\n");
	Qfloat q1;
	ScanQfloat(q1);
	Qfloat q2;
	ScanQfloat(q2);

	printf("Cong:\n");
	Qfloat q3 = q1 + q2;
	PrintQfloat(q3);

	printf("Tru:\n");
	q3 = q1 - q2;
	PrintQfloat(q3);

	printf("Nhan:\n");
	q3 = q1 * q2;
	PrintQfloat(q3);

	printf("Chia:\n");
	q3 = q1 / q2;
	PrintQfloat(q3);
}