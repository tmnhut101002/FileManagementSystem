#include "Header.h"

int main(int argc, char** argv)
{
    // xử lí nhập tên ổ đĩa
    wstring disk_name;
    cout << "Nhap ten o dia de thuc hien doc thong tin." << endl;
    cout << "(Vi du: C, E, F, G) : " << endl;
    wcin >> disk_name;
    disk_name = L"\\\\.\\" + disk_name + L":";
    LPCWSTR drive = disk_name.c_str();
    NTFS n;
    // xử lí đọc
    BYTE* sector = new BYTE[512];
    int flag = ReadSector(drive, 0, sector);
    n = readPBS(sector);
    // nếu đọc thành công
    if (flag == 1)
    {
        // in ra bảng sector vừa đọc
        Print_Sector(sector);

        // Đọc Bios Parameter Block
        printPBS(sector, drive, n);

        // giải phóng bộ nhớ
        delete[] sector;
    }
    else
    {
        cout << "Doc khong thanh cong !!!" << endl;
        return 0;
    }
}
