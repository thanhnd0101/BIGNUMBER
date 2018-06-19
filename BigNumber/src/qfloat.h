/* So cham dong lon 128-bit
 * YEUCAU ham yeu cau tu do an
 * TODO ke hoach can lam
 * "If it ain't broke, don't fix it"
 */

const int Qfloat_bits_size = 128;

// So chu so lon nhan he so 10 bieu dien
// Qfloat_MAX_STR10 = 10.....i ; i =4928;
const int Qfloat_MAX_STR10 = 5000;

// So chu so lon nhat he so 2 bieu dien he so 10 cao nhat dang CHUAN
const int Qfloat_MAX_STR2 = 16384;

const int Qfloat_Bits_ThapPhan = 112;

// So chu so lon nhat he so 10 phan Thap Phan
// 2^112 = 5.19*10^33
const int Qfloat_Str10_ThapPhan = 33;

const int Qfloat_Bits_Mu = 15;
const int Qfloat_Bits_Sign = 1;
const int Qfloat_block_size = 4;

struct Qfloat {
	int block[4];
};

// YEUCAU: Nhap xuat
void ScanQfloat(Qfloat &x);
void PrintQfloat(Qfloat x);

// YEUCAU: Chuyen doi, mac dinh bits la 128
Qfloat BinToDec_float(bool *bit);
bool *DecToBin_float(Qfloat x);

// YEUCAU: + - * /
Qfloat operator+(Qfloat, Qfloat);
Qfloat operator-(Qfloat, Qfloat);
Qfloat operator*(Qfloat, Qfloat);
Qfloat operator/(Qfloat, Qfloat);

void testScanQfloat();
void testCongTruNhanChiaQfloat();

// Xu ly string
bool *float_str10_to_bit(char *num);
char *Qfloat_bit_to_str10(bool *bits);
