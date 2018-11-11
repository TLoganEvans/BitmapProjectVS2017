/****************************************************************************
 * File: bitmap.cpp
 * Author: Allen, P
 * Course: COP 2001, 201810, 10410
 * Purpose: bitmap image library
 ****************************************************************************/
#include "bitmap.h"
#include <math.h>
/********************************************************************
 * Bitmap Functions
 *******************************************************************/

 /********************************************************************
 * Read a bitmap structure from file
 *******************************************************************/

 /**
 * Reads a bitmap file into a BITMAP structure that contains the
 * various bitmap header structures and the pixel array.
 *
 * @param ins  - reference to an ifstream object
 * @param bfOut - output parameter of the BITMAP structure to fill
 * @return int - the number of bytes read
 */
int readBitmap(ifstream& ins, BITMAP& bmOut){
    int bytesRead = 0;
    // read the File Header
    bytesRead = readBitmapFileHeader(ins, bmOut.bf);
    if(bytesRead == sizeof(BITMAPFILEHEADER)) {
        // read the Info Header
        bytesRead += readBitmapInfoHeader(ins, bmOut.bmi.bmiHeader);
        if (bytesRead == sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)){

            // skip any color tables or other padding between the Info Header
            // and the pixel array
            if (bmOut.bf.bfOffBits > sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) {
                bytesRead += skipBytes(ins, bmOut.bf.bfOffBits - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)));
            }

            // resize the pixel array
            bmOut.bmi.bmiColors.reserve(bmOut.bmi.bmiHeader.biWidth * bmOut.bmi.bmiHeader.biHeight);

            // fill the pixel array
            bytesRead += readPixels(ins, bmOut.bmi);
        }
    }

    return bytesRead;
}


/**
 * Reads the File Header from a bitmap file and fills a BITMAPFILEHEADER
 * structure passed in.  The function returns the number of bytes read
 * to fill the structure, which should be of sizeof(BITMAPFILEHEADER).
 *
 * @param ins  - reference to an ifstream object
 * @param bfOut - output parameter of the BITMAPFILEHEADER structure to fill
 * @return int - the number of bytes read
 */
int readBitmapFileHeader(ifstream& ins, BITMAPFILEHEADER& bfOut )
{
	// read in the number of bytes of the bitmap file header (14 bytes)
	// all at once, and overwrite the whole structure, thus filling
	// each area of the structure with the correct values
	ins.read((char*)&bfOut, sizeof(BITMAPFILEHEADER));
    return (int)ins.gcount();
}


/**
 * Reads the Info Header from a bitmap file and fills a BITMAPINFOHEADER
 * structure passed in.  The function returns the number of bytes read
 * to fill the structure, which should be of sizeof(BITMAPINFOHEADER).
 *
 * @param ins  - reference to an ifstream object
 * @param biOut - output parameter of the BITMAPINFOHEADER structure to fill
 * @return int - the number of bytes read
 */
int readBitmapInfoHeader(ifstream& ins, BITMAPINFOHEADER& biOut )
{
	// read in the number of bytes of the bitmap info header (40 bytes)
	// all at once, and overwrite the whole structure, thus filling
	// each area of the structure with the correct values
	ins.read((char*)&biOut, sizeof(BITMAPINFOHEADER));
	return (int)ins.gcount();
}


/**
 * Read all pixels into the pixel array passed in.
 *
 * @param ins  - reference to an ifstream object
 * @param pixelArray - pixel array to fill
 * @return int - the number of bytes read
 */
int readPixels(ifstream& ins, BITMAPINFO& bmi){
    int bytesRead = 0;

	// calcluate how many bytes make up each row 
	int pixelPerRow = (int)floor(bmi.bmiHeader.biWidth * sizeof(RGB));
	// calcuate the amount of padding to add at the end of each row
	int rowPadding = (int)floor(bmi.bmiHeader.biSizeImage / bmi.bmiHeader.biHeight) - pixelPerRow;

	// temp byte array
	BYTE tmp[sizeof(RGB)] = { 0 };
	// for each row in the bitmap
	for (unsigned int rows = 0; rows < bmi.bmiHeader.biHeight; rows++) {
		// for each pixel in the row 
		for (unsigned int cols = 0; cols < bmi.bmiHeader.biWidth; cols++) {
			// read the next pixel into the temp array
			bytesRead += readBytes(ins, tmp, sizeof(RGB));
			// now put the pixel bytes into an RGB structure
			RGB nextPixel = { tmp[0], tmp[1], tmp[2] };
			// add the RGB structure to the pixel array
			bmi.bmiColors.push_back(nextPixel);
		}
		// add padding to the end of each row
		bytesRead += readBytes(ins, tmp, rowPadding);
	}
    return bytesRead;
}

/********************************************************************
 * Write a bitmap structure to file
 *******************************************************************/

/**
 * Write a bitmap to an external file.
 *
 * @param outFile  - reference to an ofstream object
 * @param bmOut - BITMAP structure to write
 * @return int - the number of bytes written
 */
int writeBitmap(ofstream& outFile, BITMAP bmOut) {
    int bytesWritten = 0;

    bytesWritten += writeBitmapFileHeader(outFile, bmOut.bf);
    bytesWritten += writeBitmapInfoHeader(outFile, bmOut.bmi.bmiHeader);
    int padding = bmOut.bf.bfOffBits - sizeof(BITMAPFILEHEADER) - bmOut.bmi.bmiHeader.biSize;
    bytesWritten += writeBitmapPadding(outFile, padding);
    bytesWritten += writeBitmapPixels(outFile, bmOut.bmi);
    return bytesWritten;
}

/**
 * Write the bitmap file header to file
 *
 * @param outFile  - reference to an ofstream object
 * @param bfOut - bitmap file header structure to write
 * @return int - the number of bytes written
 */
int writeBitmapFileHeader(ofstream& outFile, BITMAPFILEHEADER bfOut) {
	// write the whole bitmap file header structure as
	// a stream of bytes for the number of bytes in the structure
	outFile.write((char*)&bfOut, sizeof(BITMAPFILEHEADER));
	return sizeof(BITMAPFILEHEADER);
}


/**
 * Write the bitmap info header to file
 *
 * @param outFile  - reference to an ofstream object
 * @param biOut - bitmap info header structure to write
 * @return int - the number of bytes written
*/
int writeBitmapInfoHeader(ofstream& outFile, BITMAPINFOHEADER biOut) {
	// write the whole bitmap info header structure as
	// a stream of bytes for the number of bytes in the structure
	outFile.write((char*)&biOut, sizeof(BITMAPINFOHEADER));
	return sizeof(BITMAPINFOHEADER);
}

/**
 * Write padding to bitmap file
 *
 * @param outFile  - reference to an ofstream object
 * @param bytesOut - number of padding bytes to write
 * @return int - the number of bytes written
 */
int writeBitmapPadding(ofstream& outFile, int bytesOut) {
    int bytesWritten = 0;
	// write one padding byte at a time
    for (int i=0; i< bytesOut; i++){
        BYTE padding[1] = {0};		// temp padding array
        bytesWritten += writeBytes(outFile, padding, 1);
    }

    return bytesWritten;
}


/**
 * Write pixels to bitmap file
 *
 * @param outFile  - reference to an ofstream object
 * @param bmiOut - bitmap info header structure with pixel array
 * @return int - the number of bytes written
 */
int writeBitmapPixels(ofstream& outFile, BITMAPINFO bmiOut) {
    int bytesWritten = 0;

	// calcluate how many bytes make up each row 
	int pixelPerRow = (int)floor(bmiOut.bmiHeader.biWidth * sizeof(RGB));
	// calcuate the amount of padding to add at the end of each row
	int rowPadding = (int)floor(bmiOut.bmiHeader.biSizeImage / bmiOut.bmiHeader.biHeight) - pixelPerRow;
	// padding array
	char *pad = new char[rowPadding](); // defalut array of row padding size to all zeros

	int pixel = 0;
	// for each row in the bitmap
	for (unsigned int rows = 0; rows < bmiOut.bmiHeader.biHeight; rows++) {
		// for each pixel in the row 
		for (unsigned int cols = 0; cols < bmiOut.bmiHeader.biWidth; cols++) {
			// get the next pixel into a byte array
			BYTE bytes[sizeof(RGB)] = { bmiOut.bmiColors[pixel].rgbBlue, bmiOut.bmiColors[pixel].rgbGreen, bmiOut.bmiColors[pixel].rgbRed };
			// write the byte array out
			bytesWritten += writeBytes(outFile, bytes, sizeof(RGB));
			pixel++;
		}
		// write row padding after each row
		outFile.write(pad, rowPadding);
		bytesWritten += rowPadding;
	}

    return bytesWritten;
}


/********************************************************************
 * Print a bitmap structure
 *******************************************************************/

 /**
 * Prints a BITMAPFILEHEADER structure to stdout.
 *
 * @param bfPrint - BITMAPFILEHEADER to print
 */
void printBitmapFileHeader(BITMAPFILEHEADER bfPrint){
    cout << "BITMAPFILEHEADER" << endl;
    cout << '\t' << "bfType: " << (char)(bfPrint.bfType & 0X00FF) << (char)(bfPrint.bfType >> 8) << endl;
    cout << '\t' << "bfSize: " << bfPrint.bfSize << endl;
    cout << '\t' << "bfReserved1: " << bfPrint.bfReserved1 << endl;
    cout << '\t' << "bfReserved2: " << bfPrint.bfReserved2 << endl;
    cout << '\t' << "bfOffBits: " << bfPrint.bfOffBits << endl;
}

/**
 * Prints a BITMAPINFOHEADER structure to stdout.
 *
 * @param biPrint - BITMAPINFOHEADER to print
 */
void printBitmapInfoHeader(BITMAPINFOHEADER biPrint){
    cout << "BITMAPINFOHEADER" << endl;
    cout << '\t' << "biSize:" << biPrint.biSize << endl;
    cout << '\t' << "biWidth:" << biPrint.biWidth << endl;
    cout << '\t' << "biHeight:" << biPrint.biHeight << endl;
    cout << '\t' << "biPlanes:" << biPrint.biPlanes << endl;
    cout << '\t' << "biBitCount:" << biPrint.biBitCount << endl;
    cout << '\t' << "biCompression:" << biPrint.biCompression << endl;
    cout << '\t' << "biSizeImage:" << biPrint.biSizeImage << endl;
    cout << '\t' << "biXPelsPerMeter:" << biPrint.biXPelsPerMeter << endl;
    cout << '\t' << "biYPelsPerMeter:" << biPrint.biYPelsPerMeter << endl;
    cout << '\t' << "biClrUsed:" << biPrint.biClrUsed << endl;
    cout << '\t' << "biClrImportant:" << biPrint.biClrImportant << endl;
}

/**
 * Print a single RGB pixel. 
 * (most bitmaps are going to be too large to print all of the pixels
 * to the console, but this function can be called in a loop for each pixel
 * you want printed out)
 *
 * @param pixel - the RGB pixel to print
 */
void printPixel(RGB pixel){
    cout << "rgb(" << (unsigned int)pixel.rgbRed << ", " << (unsigned int)pixel.rgbGreen << ", " << (unsigned int)pixel.rgbBlue << ")";
}




