#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

inline const char* getFileContent(const char* path)
{
	try
	{
		std::ifstream input(path, std::ios::binary);

		// copy all data into buffer
		const std::vector buffer(std::istreambuf_iterator(input), {});
		return buffer.data();
	}
	catch (errno_t err)
	{
		std::cerr << "Error reading file :" << path << ", errno " << err << std::endl;
	}
	return nullptr;
}

inline void dump16(char* str, const int len)
{
	char* p = str;
	for (int n = 0; n < len; ++n)
	{
		printf("%2.2x ", *p);
		++p;
	}

	printf("\t");

	p = str;
	for (int n = 0; n < len; ++n)
	{
		printf("%c", *p ? *p : ' ');
		++p;
	}
	printf("\n");
}