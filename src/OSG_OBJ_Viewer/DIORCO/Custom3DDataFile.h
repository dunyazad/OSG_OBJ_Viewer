#pragma once

#include <string>
#include <fstream>

using namespace std;

class Custom3DDataFile {

public:
	Custom3DDataFile();
	~Custom3DDataFile();

	void Open(string fileName, bool isBinary);
	void Close();
	bool isOpen();

	bool GetWord(string& word);

	void Read(char* buffer, int length);
	void Write(char* buffer, int length);

	fstream& operator << (bool data);
	fstream& operator << (short data);
	fstream& operator << (unsigned short data);
	fstream& operator << (int data);
	fstream& operator << (unsigned int data);
	fstream& operator << (long data);
	fstream& operator << (unsigned long data);
	fstream& operator << (float data);
	fstream& operator << (double data);
	fstream& operator << (string& data);
	fstream& operator << (const string& data);
	fstream& operator << (char* data);
	fstream& operator << (const char* data);
	
private:
	fstream* m_pFileStream;
	string m_stlFileName;
};
