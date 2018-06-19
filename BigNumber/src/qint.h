/* So nguyen lon 128-bit
 * bieu dien tu -2^127 den 2^127 - 1
 * YEUCAU ham yeu cau tu do an
 * TODO ke hoach can lam
 * "If it ain't broke, don't fix it"
 */
#ifndef QINT_INCLUDED
#define QINT_INCLUDED

struct QInt {
	int block[4];
};

const int QInt_Size = 128;

bool *str2_to_bit(char *num, int size);
char *bit_to_str2(bool *bits, int size);
// TODO str16
bool *str16_to_bit(char *num, int size);
char *bit_to_str16(bool *bits);

// YEUCAU: Nhap xuat
void ScanQInt(QInt &q);
void PrintQInt(QInt q);

// YEUCAU: Chuyen doi, mac dinh bits la 128
bool *DecToBin_int(QInt q);
QInt BinToDec_int(bool *bits);
char *BinToHex_int(bool *bits);
char *DecToHex_int(QInt q);

/* YEUCAU: Xu ly toan tu + - * /
 * tran so cua cong: am + am = duong va duong + duong = am
 * tru la nguoc lai cua cong
 * nhan su dung booth
 */
QInt operator+(QInt a, QInt b);
QInt operator-(QInt a, QInt b);
QInt operator*(QInt Q, QInt M);
QInt operator/(QInt Q, QInt M);

/* YEUCAU Xu ly toan tu AND (&), OR(|), XOR(^), NOT(~)
 * dich trai va dich phai
 */
QInt operator&(QInt a, QInt b);
QInt operator|(QInt a, QInt b);
QInt operator^(QInt a, QInt b);
QInt operator~(QInt a);
QInt operator<<(QInt a, int count);
QInt operator>>(QInt a, int count);

// Cac ham kiem tra
void test_input_convert_int();
void test_cong_tru_int();
void test_nhan_chia_int();
void test_bit_operator_int();

#endif
