#include "xulyfile.h"
#include "qfloat.h"
#include "qint.h"
#include "share.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **chia_thanh_toan_hang(char *line, int &count);
bool *tinh_toan_2_ngoi_int(char **str, bool *bits_1, bool *bits_3);
char *chuyen_co_so_int(char **str, bool *bits);

bool *tinh_toan_2_ngoi_float(char **str, bool *bits_1, bool *bits_3);

void file_qint(char *input, char *output)
{
	FILE *f_in = fopen(input, "r");
	if (!f_in) {
		printf("Khong ton tai file %s\n", input);
		return;
	}
	FILE *f_out = fopen(output, "w");
	const int MAX = 200;
	char line[MAX];
	while (fgets(line, MAX, f_in)) {
		int count;
		char **str = chia_thanh_toan_hang(line, count);
		bool *bits_kq = NULL;
		char *chars_kq = NULL;
		// o1 o2 o3 o4
		if (count == 4) {
			// Khi shift a << b, b la int
			if (strcmp(str[2], "<<") == 0 ||
			    strcmp(str[2], ">>") == 0) {
				int shift_amount = atoi(str[3]);
				bool *bits = NULL;
				if (strcmp(str[0], "10") == 0) {
					bits = str10_to_bit(str[1], QInt_Size);
				} else if (strcmp(str[0], "2") == 0) {
					bits = str2_to_bit(str[1], QInt_Size);
				} else if (strcmp(str[0], "16") == 0) {
					bits = str16_to_bit(str[1], QInt_Size);
				}
				QInt q = BinToDec_int(bits);
				QInt q_kq;
				if (strcmp(str[2], "<<") == 0) {
					q_kq = q << shift_amount;
				} else {
					q_kq = q >> shift_amount;
				}
				bits_kq = DecToBin_int(q_kq);
				if (bits)
					free(bits);
			} else {
				bool *bits_1 = NULL, *bits_3 = NULL;
				if (strcmp(str[0], "10") == 0) {
					bits_1 =
					    str10_to_bit(str[1], QInt_Size);
					bits_3 =
					    str10_to_bit(str[3], QInt_Size);
				} else if (strcmp(str[0], "2") == 0) {
					bits_1 = str2_to_bit(str[1], QInt_Size);
					bits_3 = str2_to_bit(str[3], QInt_Size);
				} else if (strcmp(str[0], "16") == 0) {
					bits_1 =
					    str16_to_bit(str[1], QInt_Size);
					bits_3 =
					    str16_to_bit(str[3], QInt_Size);
				}
				bits_kq =
				    tinh_toan_2_ngoi_int(str, bits_1, bits_3);
				if (bits_1)
					free(bits_1);
				if (bits_3)
					free(bits_3);
			}

			if (strcmp(str[0], "10") == 0) {
				chars_kq = bit_to_str10(bits_kq, QInt_Size);
			} else if (strcmp(str[0], "2") == 0) {
				chars_kq = bit_to_str2(bits_kq, QInt_Size);
			} else if (strcmp(str[0], "16") == 0) {
				chars_kq = bit_to_str16(bits_kq);
			}
		} else if (count == 3) {
			bool *bits = NULL;
			if (strcmp(str[0], "10") == 0) {
				bits = str10_to_bit(str[2], QInt_Size);
			} else if (strcmp(str[0], "2") == 0) {
				bits = str2_to_bit(str[2], QInt_Size);
			} else if (strcmp(str[0], "16") == 0) {
				bits = str16_to_bit(str[2], QInt_Size);
			}
			if (strcmp(str[1], "~") == 0) {
				QInt q = BinToDec_int(bits);
				QInt q_kq = ~q;
				bits_kq = DecToBin_int(q_kq);
			} else {
				chars_kq = chuyen_co_so_int(str, bits);
			}
			if (bits)
				free(bits);
		}

		fprintf(f_out, "%s\n", chars_kq);

		// free memory
		if (bits_kq)
			free(bits_kq);
		if (chars_kq)
			free(chars_kq);
		for (int i = 0; i < count; ++i) {
			free(str[i]);
		}
		if (str)
			free(str);
	}
	fclose(f_in);
	fclose(f_out);
}

char **chia_thanh_toan_hang(char *line, int &count)
{
	// line co dang o1 o2 o3 o4 (2 3 + 5)
	// hoac o1 o2 o3 (2 16 5)
	int len = strlen(line);
	int first_w = 0;
	int space = 0;
	char **str = NULL; // luu o1 o2 ...
	count = 0;	 // luu bao nhieu
	while (space < len) {
		if (isspace(line[space])) {
			str =
			    (char **)realloc(str, sizeof(char *) * (count + 1));
			str[count] = (char *)malloc(sizeof(char) *
						    (space - first_w + 1));
			for (int i = first_w; i < space; ++i) {
				str[count][i - first_w] = line[i];
			}
			str[count][space - first_w] = '\0';
			++count;
			first_w = space + 1;
		}
		++space;
	}
	return str;
}

bool *tinh_toan_2_ngoi_int(char **str, bool *bits_1, bool *bits_3)
{
	QInt q_1 = BinToDec_int(bits_1);
	QInt q_3 = BinToDec_int(bits_3);
	QInt q_kq;
	if (strcmp(str[2], "+") == 0) {
		q_kq = q_1 + q_3;
	} else if (strcmp(str[2], "-") == 0) {
		q_kq = q_1 - q_3;
	} else if (strcmp(str[2], "*") == 0) {
		q_kq = q_1 * q_3;
	} else if (strcmp(str[2], "/") == 0) {
		q_kq = q_1 / q_3;
	} else if (strcmp(str[2], "&") == 0) {
		q_kq = q_1 & q_3;
	} else if (strcmp(str[2], "|") == 0) {
		q_kq = q_1 | q_3;
	}
	return DecToBin_int(q_kq);
}

char *chuyen_co_so_int(char **str, bool *bits)
{
	if (strcmp(str[1], "10") == 0) {
		return bit_to_str10(bits, QInt_Size);
	} else if (strcmp(str[1], "2") == 0) {
		return bit_to_str2(bits, QInt_Size);
	}
	return bit_to_str16(bits);
}

void file_qfloat(char *input, char *output)
{
	FILE *f_in = fopen(input, "r");
	if (!f_in) {
		printf("Khong ton tai file %s\n", input);
		return;
	}
	FILE *f_out = fopen(output, "w");
	const int MAX = 5000;
	char line[MAX];
	while (fgets(line, MAX, f_in)) {
		int count;
		char **str = chia_thanh_toan_hang(line, count);
		bool *bits_kq = NULL;
		char *chars_kq = NULL;
		// o1 o2 o3 o4
		if (count == 4) {
			bool *bits_1 = NULL;
			bool *bits_3 = NULL;
			if (strcmp(str[0], "10") == 0) {
				bits_1 = float_str10_to_bit(str[1]);
				bits_3 = float_str10_to_bit(str[3]);
			}
			bits_kq = tinh_toan_2_ngoi_float(str, bits_1, bits_3);
			if (bits_1)
				free(bits_1);
			if (bits_3)
				free(bits_3);

			// Tra ve char * lay ket qua
			if (strcmp(str[0], "10") == 0) {
				chars_kq = Qfloat_bit_to_str10(bits_kq);
			}
		}

		fprintf(f_out, "%s\n", chars_kq);

		// free memory
		if (bits_kq)
			free(bits_kq);
		if (chars_kq)
			free(chars_kq);
		for (int i = 0; i < count; ++i) {
			free(str[i]);
		}
		if (str)
			free(str);
	}

	fclose(f_in);
	fclose(f_out);
}

// 10 1.25 / 0.05
bool *tinh_toan_2_ngoi_float(char **str, bool *bits_1, bool *bits_3)
{
	Qfloat q_1 = BinToDec_float(bits_1);
	Qfloat q_3 = BinToDec_float(bits_3);
	Qfloat q_kq;
	if (strcmp(str[2], "+") == 0) {
		q_kq = q_1 + q_3;
	} else if (strcmp(str[2], "-") == 0) {
		q_kq = q_1 - q_3;
	} else if (strcmp(str[2], "*") == 0) {
		q_kq = q_1 * q_3;
	} else if (strcmp(str[2], "/") == 0) {
		q_kq = q_1 / q_3;
	}
	return DecToBin_float(q_kq);
}