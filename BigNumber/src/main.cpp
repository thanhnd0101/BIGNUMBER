#include "qfloat.h"
#include "qint.h"
#include "share.h"
#include "xulyfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 4 ||
	    (strcmp(argv[1], "int") != 0 && strcmp(argv[1], "float")) != 0) {
		printf("Nhap lai theo cau truc\n");
		printf(
		    "1612180_1612628_1612403.exe int file_input file_output\n");
		printf("1612180_1612628_1612403.exe float file_input "
		       "file_output\n");
		return 0;
	}
	if (strcmp(argv[1], "int") == 0) {
		file_qint(argv[2], argv[3]);
	} else if (strcmp(argv[1], "float") == 0) {
		file_qfloat(argv[2], argv[3]);
	}
	return 0;
}
