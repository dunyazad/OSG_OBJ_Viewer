#include "stdafx.h"
#include "Custom3DDataFile.h"

Custom3DDataFile::Custom3DDataFile()
	: m_pFileStream(nullptr)
{
	m_pFileStream = new fstream();
}

Custom3DDataFile::~Custom3DDataFile()
{
	if(m_pFileStream != nullptr) {
		delete m_pFileStream;
	}
}

void Custom3DDataFile::Open(string fileName, bool isBinary)
{
	if(isBinary) {
		(*m_pFileStream).open(fileName, ios::binary | ios::in | ios::out | ios::trunc);
	} else {
		(*m_pFileStream).open(fileName, ios::in | ios::out | ios::trunc);
	}
}

void Custom3DDataFile::Close()
{
	(*m_pFileStream).close();
}

bool Custom3DDataFile::isOpen()
{
	return (*m_pFileStream).is_open();
}

bool Custom3DDataFile::GetWord(string& word)
{
	return (*m_pFileStream) >> word;
}

void Custom3DDataFile::Read(char* buffer, int length)
{
	(*m_pFileStream).read(buffer, length);
}

void Custom3DDataFile::Write(char* buffer, int length)
{
	(*m_pFileStream).write(buffer, length);
}

fstream& Custom3DDataFile::operator << (bool data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (short data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (unsigned short data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (int data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (unsigned int data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (long data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (unsigned long data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (float data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (double data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (string& data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (const string& data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (char* data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& Custom3DDataFile::operator << (const char* data)
{
	return (fstream&)((*m_pFileStream) << data);
}
