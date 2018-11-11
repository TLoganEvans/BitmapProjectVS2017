/*****************************************************************************
* File: bytes.cpp
* Author: Allen, P.
* Course: COP 2001, 201801, 10410
* Description: Functions for BYTE handeling library
*****************************************************************************/
#include "bytes.h"

/********************************************************************
 * BYTE, WORD, and DWORD Conversion Functions
 *******************************************************************/
 /**
 * Convert two bytes to a WORD value.
 *
 * @param byte1 - the low BYTE
 * @param byte2 - the high BYTE
 * @return int - the WORD value of the two bytes
 */
int bytesToWORD(BYTE byte1, BYTE byte2) {
	// call the bytesToDWORD passing in the low and high bytes of
	// the low WORD
	return bytesToDWORD(byte1, byte2, 0, 0);
}

/**
* Convert four bytes to a DWORD value.
*
* @param byte1 - the low BYTE of the low WORD
* @param byte2 - the high BYTE of the low WORD
* @param byte3 - the low BYTE of the high WORD
* @param byte4 - the high BYTE of the high WORD
* @return int - the DWORD value of the four bytes
*/
int bytesToDWORD(BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4) {
	// left shift each byte by 8 to align it with its position
	// in the full DWORD value
	// (i.e. multiply each value by 2^8 * position)
	return (byte4 << 24) + (byte3 << 16) + (byte2 << 8) + byte1;
}


/**
* Convert a WORD value to an array of bytes.
*
* @param wordVal - WORD size value
* @param byte1 - the low BYTE of the low WORD
* @param byte2 - the high BYTE of the low WORD
* @return char[] - array of bytes
*/
void WORDtoBytes(WORD wordVal, BYTE& byte1, BYTE& byte2) {
	BYTE byte3, byte4;
	// call the DWORD to bytes function passing
	// the word value passed in
	return DWORDtoBytes((unsigned int)wordVal, byte1, byte2, byte3, byte4);
}

/**
* Convert a DWORD value to an array of bytes.
*
* @param dwordVal - DWORD size value
* @param byte1 - the low BYTE of the low WORD
* @param byte2 - the high BYTE of the low WORD
* @param byte3 - the low BYTE of the high WORD
* @param byte4 - the high BYTE of the high WORD
* @return char[] - array of bytes
*/
void DWORDtoBytes(DWORD dwordVal, BYTE& byte1, BYTE& byte2, BYTE& byte3, BYTE& byte4) {
	// get low byte of low word
	byte1 = (char)(dwordVal & 0X000FF);

	// get high byte of low word
	byte2 = (char)((dwordVal >> 8) & 0X00FF);

	// get low byte of high word
	byte3 = (char)((dwordVal >> 16) & 0X00FF);

	// get high byte of high word
	byte4 = (char)((dwordVal >> 24) & 0X00FF);
}


/********************************************************************
 * BYTE I/O Functions
 *******************************************************************/
 /**
 * Attempts to read n-bytes from the input stream and returns
 * the number of bytes successfully read.
 *
 * @param ins - the input stream to read from
 * @param bytes - the array of bytes to fill
 * @param bytesToRead - the number of bytes to read
 * @return int - the number of bytes read
 */
int readBytes(ifstream& ins, unsigned char bytes[], int bytesToRead) {
	int bytesRead = 0;

	char next = ' ';
	while (!ins.fail() && bytesRead < bytesToRead) {
		// get the next character from the stream
		ins.get(next);		
		if (!ins.fail()) {
			bytes[bytesRead] = (unsigned char)next;
			bytesRead++;
		}
	}
	return bytesRead;
}

/**
* Move the read pointer forward without storing bytes read.
*
* @param ins - the input stream to read from
* @param bytesToSkip - the number of bytes to strip off the input stream
* @return int - the number of bytes read
*/
int skipBytes(ifstream& ins, int bytesToSkip) {
	int bytesRead = 0;
	unsigned char temp[1];		// temp char array
	
	// just call readBytes to read one char
	// with a temp array for each byte to skip
	for (int i = 0; i < bytesToSkip; i++) {
		bytesRead += readBytes(ins, temp, 1);
	}
	return bytesRead;
}


/**
* Attempts to wite n-bytes to the output stream and returns
* the number of bytes successfully written.
*
* @param outs - the output stream to write to
* @param bytesOut - the array of bytes to write
* @param bytesToWrite - the number of bytes to write from the array
* @return int - the number of bytes written
*/
int writeBytes(ofstream& outs, unsigned char bytesOut[], int bytesToWrite) {
	int bytesWritten = 0;

	while (!outs.fail() && bytesWritten < bytesToWrite) {
		outs.put(bytesOut[bytesWritten]);
		if (!outs.fail()) {
			bytesWritten++;
		}
	}
	return bytesWritten;
}
