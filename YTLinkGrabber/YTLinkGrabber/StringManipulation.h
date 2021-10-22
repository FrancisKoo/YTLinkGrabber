#pragma once

class StringManipulation
{
public:
	static std::vector<std::wstring> split(std::wstring& s, std::wstring delimiter);
	static void reverse(std::wstring& s);
	static std::wstring slice(std::wstring& s, int b);
	static void swap(std::wstring& s, int b);
	static std::wstring GetQueryParam(std::wstring& query, std::wstring paramName);
	static void RemoveLineBreaks(std::wstring& str);
	static void RemoveSpaces(std::wstring& str);
};
