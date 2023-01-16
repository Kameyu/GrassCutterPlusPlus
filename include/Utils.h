#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

#define NOMINMAX // prevents mongocxx conflict with Windows.h
#include <Windows.h>


inline const char* getFileContent(const char* path)
{
	try
	{
		std::ifstream input(path, std::ios::binary);

		// copy all data into buffer
		const std::vector buffer(std::istreambuf_iterator(input), {});
		return buffer.data();
		/* NOTE: return const buffer on purpose
		 * cons of such an operation : if not used for a long time, buffer might be deallocated
		 * pros : faster processing and lower memory cost. If edit needed we can still copy the buffer
		 */
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

inline int abilityHash(const std::string& str) {
	int v7 = 0;
	int v8 = 0;
	while (v8 < static_cast<int>(str.length())) {
		v7 = str.at(v8++) + 131 * v7;
	}
	return v7;
}