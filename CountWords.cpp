// CountWordsFromFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
//#include <string>
#include <list>
#include <map>

using namespace std;

class CStringComparator
{
public:

	/*
	A < B --> true
	*/
	bool operator()(const char* A, const char* B) const
	{
		while (true)
		{
			if (A[0] == B[0])
			{
				//A = B
				if (!A[0])
					return false;

				A++;
				B++;
			}
			else
			{
				return A[0] < B[0];
			}
		}
	}
};

template <class T>
class CMyAllocator
{
public:
	typedef typename T value_type;

	CMyAllocator()
	{

	}

	template <class U>
	CMyAllocator(const CMyAllocator<U> &V)
	{

	}

	T* allocate(size_t Count)
	{
		//printf("Allocate %d\n", (int)(Count * sizeof(T)));

		return (T*)malloc(sizeof(T) * Count);
	}

	void deallocate(T* V, size_t Count)
	{
		//printf("Free %d\n", (int)(Count * sizeof(T)));

		free(V);
	}
};

HANDLE open_for_read() {
	return CreateFile(TEXT("ReadMe.txt"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}


int read_file(char** text) {

	HANDLE file = open_for_read();

	if (INVALID_HANDLE_VALUE == file) {
		return -1;
	}

	LARGE_INTEGER size;
	GetFileSizeEx(file, &size);
	__int64 size_n = size.QuadPart;
	CloseHandle(file);

	file = open_for_read();
	unsigned long success_bytes;

	char* buffer = new char[size_n + 1];
	ReadFile(file, buffer, size_n, &success_bytes, NULL);

	if (size_n != success_bytes) {
		CloseHandle(file);
		cout << "Expected " << success_bytes << " bytes in file, but actually was "
			<< size.LowPart << "." << endl;
		getchar();
		return -1;
	}

	CloseHandle(file);

	buffer[size_n] = '\0';
	*text = buffer;
	return success_bytes + 1;
}

void parsewords(char* text) {

	while (*text != '\0') {
		if (*text == ' ') {
			*text = '\0';
		}
		text++;
	}
}

int main(int argc, char* argv[])
{
	map<char*, size_t, CStringComparator, CMyAllocator<char*>> Map;

	char* text;
	int length = read_file(&text);

	parsewords(text);
	
	char *p = text;

	while (p - text < length) {
		while (*p == '\0')
			p++;
		if (p - text >= length)
			break;

		auto It = Map.find(p);
		if (It == Map.end())
		{
			Map.insert(make_pair(p, 1));
		}
		else
		{
			It->second++;
		}
		
		while (*p != '\0')
			p++;
	}

	for (auto Entry : Map)
	{
		printf("Word %s, count %I64d\n", Entry.first, (uint64_t)Entry.second);
	}



	/*for (int i = 0; i < length; i++)
		if (text[i] == '\0')
			cout << "[\\0]";
		else
			cout << text[i];*/


	//printcounts(&text[0], &text[length - 1]);

	getchar();

	return 0;
}