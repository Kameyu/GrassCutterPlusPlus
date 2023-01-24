#pragma once
#include <json/value.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <vector>

#define NOMINMAX
#include <Windows.h>

enum E_ErrorLevel
{
	NORMAL,
	IMPORTANT,
	CRITICAL
};

inline std::string LoadResource(const int id, const wchar_t* type)
{
	if (const HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(id), type))
	{
		if (const HGLOBAL hResourceData = LoadResource(nullptr, hResource))
		{
	        if (const LPVOID pResourceData = LockResource(hResourceData))
			{
		        const DWORD dwSize = SizeofResource(nullptr, hResource);
                std::string data(static_cast<const char*>(pResourceData), dwSize);
				UnlockResource(hResourceData);
				FreeResource(hResourceData);
				return data;
			}
		}
	}
	return {};
}

inline std::string getFileContent(const std::string& path)
{
	try
	{
		std::ifstream input(path, std::ios::binary);

		if (!input.good())
			throw std::runtime_error("Could not open file.");

		// copy all data into buffer
		const std::vector buffer(std::istreambuf_iterator(input), {});
		return buffer.data();
		/* NOTE: return const buffer on purpose
		 * cons of such an operation : if not used for a long time, buffer might be deallocated
		 * pros : faster processing and lower memory cost. If edit needed we can still copy the buffer
		 */
	}
	catch (const std::exception& e)
	{
		std::cerr << "getFileContent FAILED: " << e.what() << std::endl;
	}
	return "";
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

inline void shutdown(const E_ErrorLevel err, const std::string& msg)
{
	std::string prefix;
	switch (err)
	{
	case CRITICAL:
		prefix = "[CRITICAL]";
		break;
	case IMPORTANT:
		prefix = "[IMPORTANT]";
		break;
	case NORMAL:
		prefix = "[NORMAL]";
		break;
	}
	std::cerr << prefix << "Shutdown: " << msg << std::endl;
	exit(err);
}

/**
 * \brief Finds a value in a JSON File from a path as in JavaScript
 * \param root The root of the JSON file you want to resolve the path from
 * \param path The dotted path as written in JavaScript, e.g. "databaseInfo.server.connectionUri"
 * \return A Json::Value from the specified path if found.
 */
inline Json::Value resolveJsonPath(const Json::Value &root, const std::string& path)
{
	return Json::Path::Path(path).resolve(root);
}

inline Json::Value resolveJsonPath(const Json::Value &root, const char* path)
{
	return Json::Path::Path(path).resolve(root);
}

inline std::string translate(const char* fmt, ...)
{
	std::string res = fmt;
	va_list args;
	va_start(args, fmt);

	int pos = 0; // start of fmt
	while (*fmt)
	{
		if (*fmt == '%') // found % key
		{
			std::string buf;
			switch(*(fmt+1)) // peek modifier
			{
			case 'd':
				buf = std::to_string(va_arg(args, int));
				res.replace(pos, 2, buf);
				break;

			case 's':
				buf = va_arg(args, const char*); // no need to convert to string
				res.replace(pos, 2, buf);
				break;

			case 'f':
			case 'l':
				buf = std::to_string(va_arg(args, double));
				if (*fmt == 'l' && *(fmt+2) == 'f') // if arg is %lf for long float (aka double), replace 3 chars, not 2
					res.replace(pos, 3, buf);
				else
					res.replace(pos, 2, buf);
				break;

			default:
				++pos; // continue keyword will break current iteration
				continue; // increase pos before breaking it to continue parsing
			}
		}
		++pos;
	}
	va_end(args);
	return res;
}