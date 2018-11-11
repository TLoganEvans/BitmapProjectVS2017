/*****************************************************************************
 * File: bitmap.h
 * Author: Allen, P.
 * Course: COP 2001, 201801, 10410
 * Description: header file for the bitmap library
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include "bytes.h"
using namespace std;

#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED


/********************************************************************
 * Define packing to a single byte for bitmap structures
 *******************************************************************/
#pragma pack (push, 1)

/********************************************************************
 * Defines for image width and height in pixels
 * These must be redefined in students program with actual image sizes
 *******************************************************************/

struct BITMAPFILEHEADER {  /* bf 14 bytes*/
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
};

struct BITMAPINFOHEADER {   /* bi 40 bytes */
  DWORD biSize;
  DWORD biWidth;
  DWORD biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  DWORD biXPelsPerMeter;
  DWORD biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
};

struct RGB {    /* rgb 3 bytes */
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
};


struct BITMAPINFO { /* bmi */
    BITMAPINFOHEADER bmiHeader;
    vector<RGB> bmiColors;
};

struct BITMAP { /* bm */
    BITMAPFILEHEADER bf;
    BITMAPINFO bmi;
};

/********************************************************************
 * Bitmap Functions Declarations
 *******************************************************************/

// read a bitmap
int readBitmap(ifstream& ins, BITMAP& bmOut);
int readBitmapFileHeader(ifstream& ins,BITMAPFILEHEADER& bfOut );
int readBitmapInfoHeader(ifstream& ins, BITMAPINFOHEADER& biOut );
int readPixels(ifstream& ins, BITMAPINFO& bmi);

// write a bitmap
int writeBitmap(ofstream& outFile, BITMAP bmOut);
int writeBitmapFileHeader(ofstream& outFile, BITMAPFILEHEADER bfOut);
int writeBitmapInfoHeader(ofstream& outFile, BITMAPINFOHEADER biOut);
int writeBitmapPadding(ofstream& outFile, int bytesOut);
int writeBitmapPixels(ofstream& outFile, BITMAPINFO bmiOut);


void printBitmapFileHeader(BITMAPFILEHEADER bfOut);
void printBitmapInfoHeader(BITMAPINFOHEADER biPrint);
void printPixel(RGB pixel);


/********************************************************************
* undefine packing for single byte
*******************************************************************/
#pragma pack (pop)


#endif // BITMAP_H_INCLUDED
