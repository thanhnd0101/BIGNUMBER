#ifndef SHARE_INCLUDED
#define SHARE_INCLUDED

void cong_1_bit(bool *bits, int size);
void tru_1_bit(bool *bits, int size);
void in_bit(bool *bits, int size);
void nghich_dao_bit(bool *bits, int size);
void doi_dau_bit(bool *bits, int size);
bool *copy_bits(bool *bits, int size);

// radix la co so (10, 2, 16)
bool la_chu_so(char c, int radix);

// sequences la mot day bit
int seq_to_int(bool *bits, int from, int to);
void int_to_seq(int x, bool *bits, int from, int to);
int seq_to_uint(bool *bits, int from, int to);

// Dich bit
void dich_trai_1_bit(bool *bits, int size);
void dich_phai_1_bit(bool *bits, int size);

// Tinh toan 2 bits
bool *cong_bits(bool *bits_1, bool *bits_2, int size);
bool *tru_bits(bool *bits_1, bool *bits_2, int size);
bool *nhan_bits(bool *bits_Q, bool *bits_M, int size);
bool *chia_bits(bool *Q_bits, bool *M_bits, int size, bool *&R_bits);

// str10
void cong_str10(char *A, char *B);
void nhan_2_str10(char *num);
void chia_2_str10(char *num);
char *bit_to_str10(bool *bits, int size);
bool *str10_to_bit(char *num, int size);
char *cong_str10_unsigned(char *A, char *B);
char *tru_str10_unsigned(char *A, char *B);

#endif