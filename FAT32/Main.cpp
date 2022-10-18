#include "Header.h"

int main(int argc, char** argv)
{
    FAT32 f;
    wstring disk_name;
    cout << "Nhap o dia de thuc hien doc (vd: C, D, E, F,... khong co ':' o dang sau: ): ";
    wcin >> disk_name;
    disk_name = L"\\\\.\\" + disk_name + L":";
    LPCWSTR drive = disk_name.c_str();
    f = getValues(drive);
    printBootSector(f);
    printSector(f.BootSector);
}