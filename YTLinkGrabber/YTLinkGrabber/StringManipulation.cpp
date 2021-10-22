#include "pch.h"
#include "StringManipulation.h"

std::vector<std::wstring> StringManipulation::split(std::wstring& s, std::wstring delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length(), s_len = s.length();
	std::vector<std::wstring> ret;

	while ((pos_end = s.find(delimiter, pos_start)) != std::wstring::npos)
	{
		ret.push_back(s.substr(pos_start, pos_end - pos_start));
		pos_start = pos_end + delim_len;
		if (pos_start == s_len) break;
	}
	if (pos_start < s_len - 1) ret.push_back(s.substr(pos_start));
	return ret;
}

void StringManipulation::reverse(std::wstring& s)
{
	std::reverse(s.begin(), s.end());
}

std::wstring StringManipulation::slice(std::wstring& s, int b)
{
	return s.substr(b);
}

void StringManipulation::swap(std::wstring& s, int b)
{
	size_t len = s.length();
	wchar_t c = s[0];
	s[0] = s[b % len];
	s[b % len] = c;
}

std::wstring StringManipulation::GetQueryParam(std::wstring& query, std::wstring paramName)
{
	winrt::Windows::Foundation::WwwFormUrlDecoder decoder(query);
	winrt::hstring ret;
	try
	{
		ret = decoder.GetFirstValueByName(paramName);
	}
	catch (...)
	{
		ret = L"";
	}
	return ret.c_str();
}

void StringManipulation::RemoveLineBreaks(std::wstring& str)
{
	size_t j = 0;
	size_t strLen = str.length();
	for (size_t i = 0; i < strLen; i++)
	{
		if (str[i] != L'\r' && str[i] != L'\n')
		{
			str[j] = str[i];
			j++;
		}
	}
	if (j < strLen) str.erase(j);
}

void StringManipulation::RemoveSpaces(std::wstring& str)
{
	str.erase(std::remove(str.begin(), str.end(), L' '), str.end());
}
