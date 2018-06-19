================================================
Đồ án số lớn (số nguyên lớn và số chấm động lớn)
================================================

Số nguyên lớn
=============

| Thiết kế số nguyên 128 bits = int[4]
| Chọn mảng int[4] để mô tả QInt
| Phạm vi biểu diễn: -2^127 đến 2^127 - 1

::

        struct QInt
        {
                int block[4];
        };

Số chấm động lớn
================

Số chấm động lưu dưới dạng dãy 128 bits:

- Bit đầu tiên lưu dấu (là 0 nếu là số dương, là 1 nếu là số âm)
- 15 bits tiếp theo lưu số mũ dưới dạng số bias
- 112 bits còn lại lưu phần trị của số chấm động

| Phạm vi biểu diễn:
| Đoạn < 0: -(2 - 2^(-112)) x 2^16383 đến -(2^(-16382))
| Đoạn > 0: 2^-16382  đến (2 - 2^(-112)) x 2^16383

::

        struct Qfloat
        {
        int block[4];
        };

Sử dụng
=======

Compile ::

        make

Run ::

        1612180_1612628_1612403.exe int file_input file_output
        1612180_1612628_1612403.exe float file_input file_output
 
Người thực hiện:
===========================================================
| Tran Hau [https://github.com/SummerSad]
| Duy Thanh [https://github.com/thanhnguyenduy2304]
| Hoai Nam [https://github.com/NamTran1998]
