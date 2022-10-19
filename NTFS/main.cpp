#include "Header.h"

int main(int argc, char** argv)
{
    //nhap ten o dia
    wstring disk_name;
    cout << "Nhap ten o dia de thuc hien doc thong tin." << endl;
    cout << "(Vi du: C, E, F, G) : " << endl;
    wcin >> disk_name;
    disk_name = L"\\\\.\\" + disk_name + L":";
    LPCWSTR drive = disk_name.c_str();
    NTFS n;
    //doc sector
    BYTE* sector = new BYTE[512];
    int flag = ReadSector(drive, 0, sector);
    n = readPBS(sector);

    if (flag == 1)
    {
        //in bang sector vua doc
        printSector(sector);

        //doc Partition Boot Sector
        printPBS(sector, drive, n);

        delete[] sector;
    }
    else
    {
        cout << "Doc khong thanh cong !!!" << endl;
        return 0;
    }
}
