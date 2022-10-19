#pragma once
#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<vector>
#include<windows.h>
#include<iomanip>
#include<sstream>
#include<map>
#include<string>
using namespace std;

struct NTFS
{
    unsigned int bytes_per_sector;
    unsigned int sectors_per_cluster;
    unsigned int sectors_per_track;
    unsigned int numbers_of_head;
    int64_t total_sectors;
    int64_t MFTStart;
    int64_t MFTMirror_Start;
};

int ReadSector(LPCWSTR drive, int readPoint, BYTE*& sector);
void ReadSect2(LPCWSTR disk, BYTE*& DATA, unsigned int _nsect);
int64_t toNumber(BYTE* sector, int offset, int number);
NTFS readPBS(BYTE* sector);
void printPBS(BYTE* sector, LPCWSTR disk);
string toString(BYTE* DATA, int offset, int number);
string toBinary(int n);
int readEntryInfo(BYTE* Entry, int start);
int readEntryFileName(BYTE* Entry, int start, int ID);
void readEntryData(BYTE* Entry, int start);
void printTab(int tab);
string getnameFile(int id);
void printFolderTree(int a, int tab, int vt);
void printPBS(BYTE* sector, LPCWSTR disk, NTFS n);
void readMFT(unsigned int MFTStart, unsigned int sectors_per_cluster, LPCWSTR disk);
void folderTree(unsigned int len_MFT, unsigned int MFTStart, LPCWSTR disk);
void printSector(BYTE* sector);