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
    cout << "File management system type:" << type << endl;
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
    //ReadSector(f.sectors_of_bootsector * 512, f.FAT, 512);
}

void printBootSector(FAT32 f)
{
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "BOOT SECTOR : " << endl;
    //cout << "Type of File System: FAT32" << endl;
    readTypeofSystem(f.BootSector);
    cout << "Bytes/sector: " << f.bytes_per_sector << endl;
    cout << "Sector/cluster: " << f.sectors_per_cluster << endl;
    cout << "Number of sectors of BootSector: " << f.sectors_of_bootsector << endl;
    cout << "Number of FAT table: " << f.numbers_of_fats << endl;
    cout << "Number of sectors of RDET table: " << f.sector_of_RDET << endl;
    cout << "Sector/FAT table: " << f.sector_per_FAT << endl;
    cout << "First sector of FAT table: " << f.sectors_of_bootsector << endl;
    cout << "First sector of RDET: " << f.sectors_of_bootsector + f.numbers_of_fats * f.sector_per_FAT << endl;
    cout << "First sector of Data: " << f.sectors_of_bootsector + f.numbers_of_fats * f.sector_per_FAT + f.sector_of_RDET << endl;
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

int spc = 0;

atribute readatr(string atri)
{
    atribute temp;
    temp.read_only = atri[7] & 1;
    temp.hidden = atri[6] & 1;
    temp.system = atri[5] & 1;
    temp.vollabel = atri[4] & 1;
    temp.directory = atri[3] & 1;
    temp.archive = atri[2] & 1;
    return temp;
}

string toString(BYTE* sector, int position, int number)
{
    char* tmp = new char[number + 1];
    memcpy(tmp, sector + position, number);
    string s = "";
    for (int i = 0; i < number; i++)
        if (tmp[i] != 0x00 && tmp[i] != char(0xff))
            s += tmp[i];

    return s;
}

int toNumber(BYTE* sector, int position, int number)
{
    int k = 0;
    memcpy(&k, sector + position, number);
    return k;
}

string decToBinary(int n)
{
    string s = "";
    // Size of an integer is assumed to be 32 bits
    for (int i = 7; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
            s += "1";
        else
            s += "0";
    }
    return s;
}

void readFile(unsigned int start, int number)
{
    int k = 0;

    while (true)
    {
        if (number > 0)
        {
            if (number <= 512)
            {
                BYTE sector[512];
                ReadSector(L"\\\\.\\D:", start + k, sector);
                string line = toString(sector, 0x00, number);
                cout << "\t" << line << endl;
                number = number - 512;
            }
            else
            {
                BYTE sector[512];
                ReadSector(L"\\\\.\\D:", start + k, sector);
                string line = toString(sector, 0x00, 512);
                cout << "\t" << line << endl;
                number = number - 512;
                k = k + 1;
            }

        }
        else
        {
            break;
        }
    }


}

void readRDET(int SRDET)
{
    int k = 0;
    string temp = "";
    int count = 0;
    while (true)
    {
        bool checkend = false;
        BYTE sector[512];
        ReadSector(L"\\\\.\\D:", SRDET + k, sector);
        for (int i = 0; i < 512; i = i + 32)
        {
            if (sector[i] == 0xe5)
            {
                continue;
            }
            if (sector[i] == 0x00)
            {
                checkend = true;
                break;
            }
            if (sector[0x0B + i] == 0x0f)
            {

                temp = toString(sector, 0x01 + i, 10) + toString(sector, 0x0E + i, 12) + toString(sector, 0x1C + i, 4) + temp;
                count++;

            }
            else
            {
                if (count != 0)
                {
                    unsigned int high = toNumber(sector, 0x14 + i, 2);
                    unsigned int low = toNumber(sector, 0x1A + i, 2);
                    unsigned int total = high + low;
                    cout << "ten thu muc tep tin :" << temp << endl;
                    cout << "kich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << endl;
                    cout << "trang thai : ";
                    int temp2 = toNumber(sector, 0x0b + i, 1);
                    atribute temp1;
                    string temp3 = decToBinary(temp2);
                    temp1 = readatr(temp3);
                    if (temp1.archive == 1)
                    {
                        cout << "archive" << " ";
                    }
                    if (temp1.hidden == 1)
                    {
                        cout << "hidden" << " ";
                    }
                    if (temp1.system == 1)
                    {
                        cout << "system" << " ";
                    }
                    if (temp1.read_only == 1)
                    {
                        cout << "read only" << " ";
                    }
                    if (temp1.vollabel == 1)
                    {
                        cout << "vollabel" << " ";
                    }
                    if (temp1.directory == 1)
                    {
                        cout << "directory" << " ";
                    }
                    cout << endl;

                    cout << "cluster bat dau : " << total << endl;
                    if (total != 0x0)
                    {
                        cout << "Chiem cac sector :";
                        for (int i = 0; i < spc; i++)
                        {
                            cout << (total - 2) * spc - SRDET + i << " ";
                        }
                        cout << endl;
                    }
                    if (temp1.directory == 1 && temp1.system != 1)
                    {
                        cout << "tap tin thu muc con : " << endl;
                        readSDET((total - 2) * spc + SRDET);
                        cout << "------------------------" << endl;
                    }
                    if (temp[temp.length() - 1] == 't' && temp[temp.length() - 2] == 'x' && temp[temp.length() - 3] == 't' && temp1.archive == 1)
                    {
                        readFile((total - 2) * spc + SRDET, toNumber(sector, 0x1C + i, 4));
                    }
                    else if (temp[temp.length() - 1] != 't' && temp[temp.length() - 2] != 'x' && temp[temp.length() - 3] != 't' && temp1.archive == 1)
                    {
                        cout << "dung chuong trinh khac de doc " << endl;
                    }
                    temp = "";
                    count = 0;
                }
                else
                {
                    if (toString(sector, 0x08 + i, 3) != "   ")
                    {
                        unsigned int high = toNumber(sector, 0x14 + i, 2);
                        unsigned int low = toNumber(sector, 0x1A + i, 2);
                        unsigned int total = high + low;
                        cout << "ten thu muc tep tin:" << toString(sector, 0x00 + i, 8) << endl;
                        cout << "phan mo rong :" << toString(sector, 0x08 + i, 3) << endl;
                        cout << "kich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << " bytes" << endl;
                        cout << "trang thai : ";
                        int temp = toNumber(sector, 0x0b + i, 1);
                        atribute temp1;
                        string temp2 = decToBinary(temp);
                        temp1 = readatr(temp2);
                        if (temp1.archive == 1)
                        {
                            cout << "archive" << " ";
                        }
                        if (temp1.hidden == 1)
                        {
                            cout << "hidden" << " ";
                        }
                        if (temp1.system == 1)
                        {
                            cout << "system" << " ";
                        }
                        if (temp1.read_only == 1)
                        {
                            cout << "read only" << " ";
                        }
                        if (temp1.vollabel == 1)
                        {
                            cout << "vollabel" << " ";
                        }
                        if (temp1.directory == 1)
                        {
                            cout << "directory" << " ";
                        }
                        cout << endl;
                        cout << "cluster bat dau : " << total << endl;
                        if (total != 0x0)
                        {
                            cout << "Chiem cac sector :";
                            for (int i = 0; i < spc; i++)
                            {
                                cout << (total - 2) * spc - SRDET + i << " ";
                            }
                            cout << endl;
                        }
                        if (temp1.directory == 1 && temp1.system != 1)
                        {
                            cout << "tap tin thu muc con : " << endl;
                            readSDET((total - 2) * spc + SRDET);
                        }
                        cout << endl;
                        if (temp1.archive == 1 && toString(sector, 0x08 + i, 3) == "TXT")
                        {
                            readFile((total - 2) * spc + SRDET, toNumber(sector, 0x1C + i, 4));

                        }
                    }
                    else
                    {
                        unsigned int high = toNumber(sector, 0x14 + i, 2);
                        unsigned int low = toNumber(sector, 0x1A + i, 2);
                        unsigned int total = high + low;
                        cout << "ten thu muc :" << toString(sector, 0x00 + i, 8) << endl;
                        cout << "kich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << endl;
                        cout << "trang thai : ";
                        int temp = toNumber(sector, 0x0b + i, 1);
                        atribute temp1;
                        string temp2 = decToBinary(temp);
                        temp1 = readatr(temp2);
                        if (temp1.archive == 1)
                        {
                            cout << "archive" << " ";
                        }
                        if (temp1.hidden == 1)
                        {
                            cout << "hidden" << " ";
                        }
                        if (temp1.system == 1)
                        {
                            cout << "system" << " ";
                        }
                        if (temp1.read_only == 1)
                        {
                            cout << "read only" << " ";
                        }
                        if (temp1.vollabel == 1)
                        {
                            cout << "vollabel" << " ";
                        }
                        if (temp1.directory == 1)
                        {
                            cout << "directory" << " ";
                        }
                        cout << endl;
                        cout << "cluster bat dau : " << total << endl;
                        if (total != 0x0)
                        {
                            cout << "Chiem cac sector :";
                            for (int i = 0; i < spc; i++)
                            {
                                cout << (total - 2) * spc - SRDET + i << " ";
                            }
                            cout << endl;
                        }
                        if (temp1.directory == 1 && temp1.system != 1)
                        {
                            cout << "tap tin thu muc con : " << endl;
                            readSDET((total - 2) * spc + SRDET);

                        }
                    }
                }
            }

        }
        if (checkend == true)
        {
            break;
        }
        else
        {
            k++;

        }
    }
}

void readSDET(int SRDET)
{
    int k = 0;
    string temp = "";
    int count = 0;
    while (true)
    {
        bool checkend = false;
        BYTE sector[512];
        ReadSector(L"\\\\.\\D:", SRDET + k, sector);
        for (int i = 64; i < 512; i = i + 32)
        {
            if (sector[i] == 0xe5)
            {
                continue;
            }
            if (sector[i] == 0x00)
            {
                checkend = true;
                break;
            }
            if (sector[0x0B + i] == 0x0f)
            {
                temp = toString(sector, 0x01 + i, 10) + toString(sector, 0x0E + i, 12) + toString(sector, 0x1C + i, 4) + temp;
                count++;
            }
            else
            {
                if (count != 0)
                {
                    unsigned int high = toNumber(sector, 0x14 + i, 2);
                    unsigned int low = toNumber(sector, 0x1A + i, 2);
                    unsigned int total = high + low;
                    cout << "\tten tap tin thu muc :" << temp << endl;
                    cout << "\tkich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << endl;
                    cout << "\ttrang thai : ";
                    int temp2 = toNumber(sector, 0x0b + i, 1);
                    atribute temp1;
                    string temp3 = decToBinary(temp2);
                    temp1 = readatr(temp3);
                    if (temp1.archive == 1)
                    {
                        cout << "archive" << " ";
                    }
                    if (temp1.hidden == 1)
                    {
                        cout << "hidden" << " ";
                    }
                    if (temp1.system == 1)
                    {
                        cout << "system" << " ";
                    }
                    if (temp1.read_only == 1)
                    {
                        cout << "read only" << " ";
                    }
                    if (temp1.vollabel == 1)
                    {
                        cout << "vollabel" << " ";
                    }
                    if (temp1.directory == 1)
                    {
                        cout << "directory" << " ";
                    }
                    cout << endl;
                    cout << "\tcluster bat dau : " << total << endl;
                    if (total != 0x0)
                    {
                        cout << "Chiem cac sector :";
                        for (int i = 0; i < spc; i++)
                        {
                            cout << (total - 2) * spc - SRDET + i << " ";
                        }
                        cout << endl;
                    }
                    cout << endl;
                    if (temp1.directory == 1 && temp1.system != 1)
                    {
                        cout << "tap tin thu muc con : " << endl;
                        readSDET((total - 2) * spc + SRDET);
                        cout << "------------------------" << endl;
                    }
                    if (temp[temp.length() - 1] == 't' && temp[temp.length() - 2] == 'x' && temp[temp.length() - 3] == 't' && temp1.archive == 1)
                    {
                        readFile((total - 2) * spc + SRDET, toNumber(sector, 0x1C + i, 4));
                    }
                    else if (temp[temp.length() - 1] != 't' && temp[temp.length() - 2] != 'x' && temp[temp.length() - 3] != 't' && temp1.archive == 1)
                    {
                        cout << "\tdung chuong trinh khac de doc " << endl;
                    }
                    temp = "";
                    count = 0;
                }
                else
                {
                    if (toString(sector, 0x08 + i, 3) != "   ")
                    {
                        unsigned int high = toNumber(sector, 0x14 + i, 2);
                        unsigned int low = toNumber(sector, 0x1A + i, 2);
                        unsigned int total = high + low;
                        cout << "\tten tap tin  thu muc :" << toString(sector, 0x00 + i, 8) << endl;
                        cout << "\tphan mo rong :" << toString(sector, 0x08 + i, 3) << endl;
                        cout << "\tkich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << " bytes" << endl;
                        cout << "\ttrang thai : ";
                        int temp = toNumber(sector, 0x0b + i, 1);
                        atribute temp1;
                        string temp2 = decToBinary(temp);
                        temp1 = readatr(temp2);
                        if (temp1.archive == 1)
                        {
                            cout << "archive" << " ";
                        }
                        if (temp1.hidden == 1)
                        {
                            cout << "hidden" << " ";
                        }
                        if (temp1.system == 1)
                        {
                            cout << "system" << " ";
                        }
                        if (temp1.read_only == 1)
                        {
                            cout << "read only" << " ";
                        }
                        if (temp1.vollabel == 1)
                        {
                            cout << "vollabel" << " ";
                        }
                        if (temp1.directory == 1)
                        {
                            cout << "directory" << " ";
                        }
                        cout << endl;
                        cout << "\tcluster bat dau : " << total << endl;
                        if (total != 0x0)
                        {
                            cout << "Chiem cac sector :";
                            for (int i = 0; i < spc; i++)
                            {
                                cout << (total - 2) * spc - SRDET + i << " ";
                            }
                            cout << endl;
                        }
                        cout << endl;
                        if (temp1.directory == 1 && temp1.system != 1)
                        {
                            cout << "\ttap tin thu muc con : " << endl;
                            readSDET((total - 2) * spc + SRDET);
                            cout << "------------------------" << endl;
                        }
                        if (temp1.archive == 1 && toString(sector, 0x08 + i, 3) == "TXT")
                        {
                            readFile((total - 2) * spc + SRDET, toNumber(sector, 0x1C + i, 4));
                        }
                        else
                        {
                            cout << "dung chuong trinh khac de doc " << endl;
                        }
                    }
                    else
                    {
                        unsigned int high = toNumber(sector, 0x14 + i, 2);
                        unsigned int low = toNumber(sector, 0x1A + i, 2);
                        unsigned int total = high + low;
                        cout << "\tten tap thu muc :" << toString(sector, 0x00 + i, 8) << endl;
                        cout << "\tkich thuoc : " << dec << toNumber(sector, 0x1C + i, 4) << endl;
                        cout << "\ttrang thai : ";
                        int temp = toNumber(sector, 0x0b + i, 1);
                        atribute temp1;
                        string temp2 = decToBinary(temp);
                        temp1 = readatr(temp2);
                        if (temp1.archive == 1)
                        {
                            cout << "archive" << " ";
                        }
                        if (temp1.hidden == 1)
                        {
                            cout << "hidden" << " ";
                        }
                        if (temp1.system == 1)
                        {
                            cout << "system" << " ";
                        }
                        if (temp1.read_only == 1)
                        {
                            cout << "read only" << " ";
                        }
                        if (temp1.vollabel == 1)
                        {
                            cout << "vollabel" << " ";
                        }
                        if (temp1.directory == 1)
                        {
                            cout << "directory" << " ";
                        }
                        cout << endl;
                        cout << "\tcluster bat dau : " << total << endl;
                        cout << "Chiem cac sector :";
                        if (total != 0x0)
                        {
                            cout << "Chiem cac sector :";
                            for (int i = 0; i < spc; i++)
                            {
                                cout << (total - 2) * spc - SRDET + i << " ";
                            }
                            cout << endl;
                        }
                        if (temp1.directory == 1 && temp1.system != 1)
                        {
                            cout << "tap tin thu muc con : " << endl;
                            readSDET((total - 2) * spc + SRDET);
                            cout << "------------------------" << endl;
                        }
                    }
                }
            }

        }
        if (checkend == true)
        {
            break;
        }
        else
        {
            k++;

        }
    }
}

