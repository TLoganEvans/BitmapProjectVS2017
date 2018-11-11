/*****************************************************************************
 * File: bytes.h
 * Author: Allen, P.
 * Course: COP 2001, 201801, 10410
 * Description: header file for BYTE handeling library
 *****************************************************************************/
#include <fstream>
using namespace std;

/********************************************************************
 * Defines for BYTE, WORD, and DWORD data size types
 *******************************************************************/
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define BYTE_ARRAY unsigned char

/********************************************************************
 * BYTE, WORD, and DWORD Conversion Functions
 *******************************************************************/
int bytesToWORD(BYTE byte1, BYTE byte2);
int bytesToDWORD(BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4);
void WORDtoBytes(WORD wordVal, BYTE& byte1, BYTE& byte2);
void DWORDtoBytes(DWORD dwordVal, BYTE& byte1, BYTE& byte2, BYTE& byte3, BYTE& byte4);

/********************************************************************
 * BYTE I/O Functions
 *******************************************************************/
int readBytes(ifstream& ins, unsigned char bytes[], int bytesToRead);
int skipBytes(ifstream& ins, int bytesToRead);
int writeBytes(ofstream& outs, unsigned char bytesOut[], int bytesToWrite);

