#include"Header.h"

int main(int argc, char** argv)
{
    boolean quit = FALSE;
    cout << "Luu y: " << endl;
    cout << "1. O dia USB duoc doc boi code ben duoi la o E:" << endl;
    cout << "   Hay chac chan rang USB cua ban la E: va duoc format FAT32." << endl;
    cout << "2. Luu y rang USB phai co file" << endl;
    cout << "   Neu khong se anh huong den ket qua dau ra." << endl;

    while (!quit)
    {
        for (int i = 0; i <= 60; i++)
            cout << "=";
        cout << endl;
        cout << " \t \t \t \t MENU" << endl;
        cout << "1: Read FAT32 information" << endl << "2: Exit" << endl;
        cout << "-> Enter your choice: ";
        int n;
        cin >> n;
        for (int i = 0; i <= 60; i++)
            cout << "=";
        cout << endl;
        switch (n)
        {
        case 1:
        {
            //READ BOOTSECTOR/FAT/RDET FROM FAT32

            BYTE sector[512];
            BYTE temp[100][100];
            vector<vector<string>> hexarr;
            ReadSector(L"\\\\.\\F:", 0, sector);
            for (int i = 0; i < 32; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    temp[i][j] = sector[i * 16 + j];
                }
            }
            cout << "1. READ BOOT SECTOR" << endl;
            tohex(temp, hexarr);
            cout << endl;

            readFAT(hexarr);

            for (int i = 0; i <= 60; i++)
                cout << "=";
            cout << endl << "2. READ RDET/SRDET" << endl;

            long int SB = docbyte(hexarr, 14, 0, 2);
            long int SF;
            long int NF = docbyte(hexarr, 0, 1, 1);
            if (docbyte(hexarr, 6, 1, 2) != 0)
            {
                SF = docbyte(hexarr, 6, 1, 2);
            }
            else
            {
                SF = docbyte(hexarr, 4, 2, 4);
            }
            long int SRDET = SB + SF * NF;

            BYTE sector1[512];
            BYTE temp1[100][100];
            vector<vector<string>> hexarr1;

            ReadSector(L"\\\\.\\F:", SB, sector1);
            for (int i = 0; i < 32; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    temp1[i][j] = sector1[i * 16 + j];
                }
            }
            cout << endl;
            cout << "Bang FAT1:" << endl;
            tohex(temp1, hexarr1);
            cout << endl;
            for (int i = 0; i <= 60; i++)
                cout << "=";
            cout << endl << "3. FOLDER TREE" << endl;
            readRDET(SRDET);
            break;
        }
        case 2:
        {
            quit = TRUE;
            break;
        }
        default:
            cout << "Invalid input! Please enter number 1 or 2" << endl;
        }
    }

    return 0;
}