/**********************************************************
 * File: bmpProject.cpp
 * Author: Evans, Trevor
 * Created: Apr 10, 2018
 * Course: FGCU, COP 2001, 201801, 10410
 * Description: Encrypts/Decrypts strings into bitmap files
 *********************************************************/
//String used to encrypt bitmap file: "Roll an intelligence save. Just do it."

#include <iostream>
#include <fstream>
#include <string>
#include "bitmap.h"

using namespace std;

int getRunMode();
string getTextToEncrypt();
int getImageFileIn(ifstream& inFile, string& fileName);
int getImageFileOut(ofstream& outFile, string& fileNameOut);
int encryptString(string strToEnc, BITMAPINFO& bmEnc);
int decryptString(string& strEncrypted, BITMAPINFO& bmDec);
string readString(string strVar);

enum RUN_MODE{ENCRYPT = 1, DECRYPT = 2, EXIT = 3};


int main(){

	bool runWhile = false;

	while(runWhile == false){

		int run = getRunMode();

		string textToEncrypt = "";
		string decryptedString = "";

		ifstream fileToEncDec;
		string inFileName = "";

		ofstream writeFile;
		string outFileName = "";

		BITMAP bmEdit;

		int bytesReadInEnc;
		int bytesReadInDec;
		int bytesReadOut;

		int pixelsEncrypted;
		int pixelsDecrypted;

		switch(run){
		case 1:

			cout << endl << "Begin encrypting image..." << endl << endl;

			textToEncrypt = getTextToEncrypt();

			getImageFileIn(fileToEncDec, inFileName);
			
			getImageFileOut(writeFile, outFileName);

			bytesReadInEnc = readBitmap(fileToEncDec, bmEdit);

			pixelsEncrypted = encryptString(textToEncrypt, bmEdit.bmi);

			bytesReadOut = writeBitmap(writeFile, bmEdit);

			cout << "	Bytes read: " << bytesReadInEnc << endl << "	Pixels encrypted: " << pixelsEncrypted << endl
				<< "Created image: " << outFileName << endl << "	Bytes written: " << bytesReadOut << endl << endl;

			runWhile = false;
			break;

		case 2:

			cout << endl << "Begin decrypting image:" << endl << endl;

			getImageFileIn(fileToEncDec, inFileName);

			bytesReadInDec = readBitmap(fileToEncDec, bmEdit);

			pixelsDecrypted = decryptString(decryptedString, bmEdit.bmi);

			cout << "Decrypted image: " << inFileName << endl << "	Bytes read: " << bytesReadInDec << endl
				<< "	Pixels decrypted: " << pixelsDecrypted << endl << "Encrypted string: " << decryptedString << endl << endl;

			runWhile = false;
			break;

		case 3:

			cout << endl << "Exiting..." << endl << endl;

			runWhile = true;
			break;

		default:

			runWhile = false;
			break;
		}

		fileToEncDec.close();
		writeFile.close();
	}

return 0;
}


/**
 * Provides user running program with menu{choices 1-3}, asks user to input a value into int variable 
 * "runMode" '1' through '3', references value to global enumerator in switch-case statement (error 
 * check included), returns value of "runMode" based on enumerator value.
 * 
 * @int runMode - integer passed in by user through console. Integer is set to a value established by global @ENUM "RUN_MODE", returned.
 */
int getRunMode() {

	int runMode;

	cout << "Select the run mode:\n	1) Encrypt\n	2) Decrypt\n	3) Exit\n=>";
	cin >> runMode;

	switch (runMode) {
	case 1:
		runMode = ENCRYPT;
		break;
	case 2:
		runMode = DECRYPT;
		break;
	case 3:
		runMode = EXIT;
		break;
	default:
		cerr << " Invalid entry. Try a valid option: ";
		runMode = -1;
		break;
	}

	return runMode;
}


/**
 * Prompts user to enter text/string they want encrypted into '.bmp' file, cin's string, 
 * sends string through function "readString(string)" and returns string to calling function.
 * 
 * @string textEncrypt - empty string variable, replaced by user input, returned.
 */
string getTextToEncrypt() {

	string textEncrypt = "";

	cout << "Enter the text to encrypt: ";

	cin >> textEncrypt;

	return readString(textEncrypt);
}


/**
 * Takes in reference parameters "inFile" and "fileName" of types "ifstream" 
 * and "string" respectively, prompts user to input name & path of file they
 * want to encode, passes string read in through function "readString()".
 * Opens 'ifstream' based on string received from previously stated function,
 * checks for file open error, returns -1 if fail, 0 if successful. 
 *
 * @filePath - string read in from console, sent to function "readString()".
 * @fileName - string received from function "readString()", used to open in-file stream.
 */
int getImageFileIn(ifstream& inFile, string& fileName) {

	string filePath = "";

	cout << "Enter name (with path) of image file to read: ";
	cin >> filePath;

	fileName = readString(filePath);

	inFile.open(filePath, std::ios::binary);
	if (inFile.fail()) {
		cout << "Failed to open file: " << filePath << endl << endl;
		return -1;
	}

	return 0;
}


/**
 * Prompts user for file name & path, cin's string variable and sends stated string
 * to function "readString(string)", opens out-file stream (includes error check), 
 * generates new out-file if none exists.
 *
 * @fileWritePath - string variable passed through console by user, sent to 
 * function "readString(string)".
 * @fileNameOut - string variable received from function "readString(string)",
 * used to open out-file stream.
 */
int getImageFileOut(ofstream& outFile, string& fileNameOut) {

	string fileWritePath = "";

	cout << "Enter name (with path) of image file to write: ";
	cin >> fileWritePath;

	fileNameOut = readString(fileWritePath);

	outFile.open(fileNameOut, std::ios::binary);
	if (outFile.fail()) {
		cout << "Failed to open file: " << fileNameOut;
		return -1;
	}

	return 0;
}


/**
 * Takes in two parameters of types @string and @BITMAPINFO with latter passed by reference. Counter
 * @int variable used to keep place of how many pixels and which pixel is currently being encrypted with
 * the string parameter. 
 *
 * A for-loop based on string length contains a nested for-loop which counts up to 8 
 * pixels in the blue space for each char in the string. 
 *
 * Inner for loop compares boolean values based on char value and pixel value both mod 2. Booleans compared
 * to each other to determine to increment or decrement least significant digit value in '.rgbBlue'.
 * 
 * @int pixelCountEnc - incrementing @int variable, increases by 1 for every pixel
 * encoded. Used to mark place in pixel array.
 * @char charStore - variable that takes the identity of each char in string parameter,
 * modulused and referenced,
 * @BYTE rgbVal - Unsigned char variable, takes place of 'bmEnc.bmiColors[pixelCountEnc].rgbBlue',
 * cleans up code.
 * @bool charTrue - Boolean variable set to true when mod 2 == 0, compared to bool 'rgBTrue'.
 * @bool rgBTrue - Boolean variable set to true when mod 2 == 0, compared to bool 'charTrue'.
 */
int encryptString(string strToEnc, BITMAPINFO& bmEnc) {

	int pixelCountEnc = 0;

//String encryption
	for (int i = 0; i < strToEnc.length();i++){

		char charStore = strToEnc[i];

		for (int j = 0; j < 8; j++) {

			BYTE rgBValEnc = bmEnc.bmiColors[pixelCountEnc].rgbBlue;

			bool charTrue = charStore % 2 == 0;
			bool rgBTrue = rgBValEnc % 2 == 0;

			if (charTrue && !rgBTrue) {
				rgBValEnc = rgBValEnc - 1;
			}
			else if (!charTrue && rgBTrue) {
				rgBValEnc = rgBValEnc + 1;
			}

			charStore = charStore >> 1;

			bmEnc.bmiColors[pixelCountEnc].rgbBlue = rgBValEnc;

			pixelCountEnc++;
		}
	}

//Sentinel encryption
	for (int k = 0; k < 8; k++) {
		BYTE rgBValSentinel = bmEnc.bmiColors[pixelCountEnc].rgbBlue;

		if ((rgBValSentinel % 2) == 1) {
			bmEnc.bmiColors[pixelCountEnc].rgbBlue = rgBValSentinel - 1;
		}

		pixelCountEnc++;
	}

	return pixelCountEnc;
}


/**
 * Takes in two parameters of types @string& and @BITMAPINFO& with both passed by reference. Counter @int
 * variable used to keep # of pixel, returns number of pixels required to decrypt string.
 *
 * Boolean controlled while-loop contains nested for loop which reads LSB of blue pixel, references LSB with
 * even or odd status which controls value of @int 'bit'. 'Bit' is turned from a binary representation to decimal
 * and stored in @BYTE type variable, counter increments by one.
 *
 * If-else statements designate if boolean is true/false respectively based on encrypted sentinel. If the sentinel
 * is found, boolean becomes true, breaks out of loop. If not found, boolean remains false and continues building
 * a string.
 *
 * @int pixelCountDec - counter, keeps track of # pixel on.
 * @bool sentinel - boolean variable, controls outer loop flow, breaks when found.
 * @BYTE createChar - BYTE variable, stores bit value and eventually ASCII value per char.
 * @int bit - integer variable, represents LSB of blue pixel.
 */
int decryptString(string& strEncrypted, BITMAPINFO& bmDec){

	int pixelCountDec = 0;

	bool sentinel = false;

	while (!sentinel) {

		BYTE createChar = 0;

		for (int n = 0; n < 8; n++) {

			BYTE rgBValDec = bmDec.bmiColors[pixelCountDec].rgbBlue;

			int bit = 0;

			if ((rgBValDec % 2) != 0) {
				bit = 1;
			}

			createChar += (bit * pow(2, n));

			pixelCountDec++;
		}

		//Sentinel Locator.
		if (createChar == 0) {
			sentinel = true;
		}
		else {
			strEncrypted += (char)createChar;
			sentinel = false;
		}
	}	
	
	return pixelCountDec;
}


/**
 * Takes in string variable 'strVar' from calling function, runs loop while char
 * variable 'nextCh' is not a newLine character using "cin.get(char)", rebuilds
 * variable 'strVar', returns said string to calling function.
 *
 * @const char NWLN - constant char, represents newLine in string.
 * @char nextCh - empty char, modified by internal function: "cin.get(char)"
 */
string readString(string strVar){

	const char NWLN = '\n';

	char nextCh = ' ';

	while(nextCh != NWLN){

		cin.get(nextCh);

	    if(nextCh != NWLN){

	    	strVar += nextCh;
	    }
	}

	return strVar;
}
