#pragma once
#include "pch.h"
class Video
{
public:
	std::wstring url;
	std::wstring signatureCipher;
	std::wstring qualityLabel;
	std::wstring audioQuality;
	std::wstring mimeType;
	std::wstring s;
	std::wstring s2;
	std::wstring sp;
};

enum class ScramblerType
{
	Reverse,
	Swap,
	Slice
};

class ScramblerFunction
{
public:
	std::wstring functionVariable;
	std::wstring functionName;
	int parameter;
	ScramblerType type;
};
