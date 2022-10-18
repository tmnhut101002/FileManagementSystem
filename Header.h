#include <windows.h>
#include <iostream>
#include<stdio.h>
#include<vector>
#include <algorithm>
#include<sstream>
#include<iomanip>
#include<cmath>
#include <fstream>
using namespace std;

#define COL 100

struct atribute
{
	int read_only;
	int hidden;
	int system;
	int vollabel;
	int directory;
	int archive;
};

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);
void tohex(BYTE data[][COL], vector<vector<string>>& a);
string typeofFAT(vector<vector<string>> a);
bool isNumber(const string& str);
long int docbyte(vector<vector<string>> a, int n, int m, int sobytecandoc);
void readBootSector(vector<vector<string>> hexarr);
void readFAT(vector<vector<string>> hexarr);
void readRDET(int SRDET);
string hex2ascii(const string& in, string& out);
unsigned char hexval(unsigned char c);
void readSDET(int SRDET);


