#pragma once

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iomanip>
using namespace std;

struct atribute
{
    int read_only;
    int hidden;
    int system;
    int vollabel;
    int directory;
    int archive;
};

struct FAT32 {
    LPCWSTR drive;
    unsigned int bytes_per_sector;
    unsigned int sectors_per_cluster;
    unsigned int sectors_of_bootsector;
    unsigned int numbers_of_fats;
    unsigned int sector_of_RDET;
    unsigned int sector_per_FAT;
    unsigned int first_sector_of_data;
    BYTE* FAT;
    BYTE* BootSector;
};


int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);
int64_t Get_Value_Little_Endian(BYTE* sector, int offset, int number);
void readTypeofSystem(BYTE* table);
FAT32 getValues(LPCWSTR drive);
void printBootSector(FAT32 f);
void printSector(BYTE* sector);