#include "Header.h"


int64_t getValue(BYTE* sector, int offset, int number)
{
    int64_t k = 0;
    memcpy(&k, sector + offset, number);
    return k;
}

void readTypeofSystem(BYTE* table)
{
    int offset = 0x52;
    char* tmp = new char[8];
    memcpy(tmp, table + offset, 8);
    string type = "";
    for (int i = 0; i < 8; i++)
        if (tmp[i] != 0x00 && tmp[i] != 0xFF)
            type += tmp[i];
    cout << "Loai he thong quan ly tap tin: " << type << endl;
}

FAT32 getValues(LPCWSTR drive)
{
    FAT32 f;
    f.drive = drive;
    f.BootSector = new BYTE[512];
    f.FAT = new BYTE[512];
    int i = ReadSector(f.drive, 0, f.BootSector);

    f.bytes_per_sector = getValue(f.BootSector, 0x0B, 2);
    f.sectors_per_cluster = getValue(f.BootSector, 0x0D, 1);
    f.sectors_of_bootsector = getValue(f.BootSector, 0x0E, 2);
    f.numbers_of_fats = getValue(f.BootSector, 0x10, 1);
    f.sector_of_RDET = (getValue(f.BootSector, 0x11, 2) * 32) / f.bytes_per_sector;
    f.sector_per_FAT = getValue(f.BootSector, 0x24, 4);
    f.first_sector_of_data = f.sectors_of_bootsector + f.numbers_of_fats * f.sector_per_FAT + f.sector_of_RDET;

    return f;
}

void printBootSector(FAT32 f)
{
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "BOOT SECTOR : " << endl;
    //cout << "Type of File System: FAT32" << endl;
    readTypeofSystem(f.BootSector);
    cout << "So byte tren 1 sector: " << f.bytes_per_sector << endl;
    cout << "So sector tren 1 cluster: " << f.sectors_per_cluster << endl;
    cout << "So sector cua vung Boot Sector: " << f.sectors_of_bootsector << endl;
    cout << "So bang FAT: " << f.numbers_of_fats << endl;
    cout << "So sector cua bang RDET: " << f.sector_of_RDET << endl;
    cout << "So sector tren 1 bang FAT: " << f.sector_per_FAT << endl;
    cout << "Sector dau tien cua bang FAT: " << f.sectors_of_bootsector << endl;
    cout << "Sector dau tien cua RDET: " << f.sectors_of_bootsector + f.numbers_of_fats * f.sector_per_FAT << endl;
    cout << "Sector dau tien cua Data: " << f.sectors_of_bootsector + f.numbers_of_fats * f.sector_per_FAT + f.sector_of_RDET << endl;
    cout << endl;
}

//Doc sector
int ReadSector(LPCWSTR drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Doc o dia thanh cong. Sau day la thong tin:\n");
    }
}

//In thong tin sector
void printSector(BYTE* sector)
{
    int count = 0;
    int num = 0;

    cout << "         0  1  2  3  4  5  6  7    8  9  A  B  C  D  E  F" << endl;

    cout << "0x0" << num << "0  ";
    bool flag = 0;
    for (int i = 0; i < 512; i++)
    {
        count++;

        if (i % 8 == 0)
            cout << "  ";

        printf("%02x ", sector[i]);

        if (i == 255)
        {
            flag = 1;
            num = 0;
        }

        if (i == 511) break;

        if (count == 16)
        {
            cout << endl;

            if (flag == 0)
            {
                num++;
                if (num < 10)
                    cout << "0x0" << num << "0  ";
                else
                {
                    char hex = char(num - 10 + 'A');
                    cout << "0x0" << hex << "0  ";
                }

            }
            else
            {
                if (num < 10)
                    cout << "0x1" << num << "0  ";
                else
                {
                    char hex = char(num - 10 + 'A');
                    cout << "0x1" << hex << "0  ";
                }
                num++;
            }

            count = 0;
        }
    }
    cout << endl;
}
