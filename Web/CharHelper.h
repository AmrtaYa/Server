#pragma once
#include "string"
#include "iostream"
#include <wtypes.h>
#include "SQLDefine.h"
inline WCHAR* To_SQLWCHAR(std::string str)
{
	WCHAR index[SQLMAXBUFFER];
	swprintf(index, SQLMAXBUFFER, L"%hs", str.c_str());
	return index;
}