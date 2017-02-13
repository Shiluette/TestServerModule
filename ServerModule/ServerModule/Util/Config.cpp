#include"stdafx.h"
#include"Config.h"
#include<fstream>

bool ReadFromFile(const char * file, str_t & readBuffer)
{
	ifstream _file(file);
	if (!_file.is_open()) {
		// TODO : 경로 문제 Log
		return false;
	}
	_file.seekg(0, ios::end);
	int size = _file.tellg();
	_file.seekg(0, ios::beg);
	readBuffer.resize(size);
	_file.read(&readBuffer[0], size);
	_file.close();
	return true;
}

bool loadConfig(jsonValue_t * root)
{
	str_t readBuffer;
	if (!ReadFromFile("./config.json", readBuffer)) {
		return false;
	}
	jsonReader_t reader;
	if (!reader.parse(readBuffer, *root)) {
		return false;
	}
	return true;
}

