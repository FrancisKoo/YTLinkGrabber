#include "pch.h"
#include "Video.h"
#include "StringManipulation.h"
#include "YoutubePlayerParser.h"

YoutubePlayerParser::YoutubePlayerParser(std::wstring file)
{
	if (file.empty()) throw std::runtime_error("parameter file is empty.");
	playerFile = file;
}

void YoutubePlayerParser::FindScramblerFunction()
{
	std::wregex pattern(L"[A-Za-z0-9]+\\=function\\(([A-Za-z]+)\\)\\{\\1=\\1\\.split\\(\\\"\\\"\\);[\\s\\S]+?return\\s\\1\\.join\\(\\\"\\\"\\)\\}");
	std::wsmatch match;
	if (regex_search(playerFile, match, pattern))
	{
		scramblerFunction = match.str(0);
		StringManipulation::RemoveLineBreaks(scramblerFunction);
	}
}

void YoutubePlayerParser::ExtractScramblerFunctionInfo()
{
	if (scramblerFunction.empty()) throw std::runtime_error("member scramblerFunction is empty.");
	std::wstring s = scramblerFunction;
	StringManipulation::RemoveSpaces(s);
	std::vector<std::wstring> ss = StringManipulation::split(s, L";");
	std::wregex pattern(L"^([A-Za-z0-9]+)\\.([A-Za-z0-9]+)\\([A-Za-z0-9]+,([0-9]+)\\)$");
	std::wsmatch match;
	for (size_t i = 1; i < ss.size() - 1; i++)
	{
		if (regex_match(ss[i], match, pattern))
		{
			ScramblerFunction sf;
			sf.functionVariable = match.str(1);
			sf.functionName = match.str(2);
			sf.parameter = std::stoi(match.str(3));
			scramblerFunctions.push_back(sf);
		}
	}
}

void YoutubePlayerParser::FindScramblerAlgorithm()
{
	if (scramblerFunctions.size() == 0) throw std::runtime_error("No scrambler functions present.");
	std::wregex pattern(L"var\\s" + scramblerFunctions[0].functionVariable + L"=\\{([\\s\\S]+?)\\}\\};");
	std::wsmatch match;
	if (regex_search(playerFile, match, pattern))
	{
		scramblerAlgorithm = match.str(1);
		StringManipulation::RemoveLineBreaks(scramblerAlgorithm);
	}
}

void YoutubePlayerParser::MatchAlgorithmWithFunction()
{
	if (scramblerAlgorithm.empty()) throw std::runtime_error("member scramblerAlgorithm is empty.");
	std::wstring s = scramblerAlgorithm;
	StringManipulation::RemoveSpaces(s);
	std::vector<std::wstring> ss = StringManipulation::split(s, L"},");
	std::map<std::wstring, ScramblerType> dic;
	for (std::wstring& func : ss)
	{
		size_t i = func.find(L':');
		if (i == std::wstring::npos) continue;
		std::wstring name = func.substr(0, i);
		if (func.find(L"reverse") != std::wstring::npos) dic[name] = ScramblerType::Reverse;
		else if (func.find(L"splice") != std::wstring::npos) dic[name] = ScramblerType::Slice;
		else dic[name] = ScramblerType::Swap;
	}
	for (ScramblerFunction& f : scramblerFunctions)
	{
		f.type = dic[f.functionName];
	}
}

void YoutubePlayerParser::ApplySignature(Video& v)
{
	if (v.s.empty()) throw std::runtime_error("argument s is empty.");
	v.s2 = v.s;
	for (ScramblerFunction& f : scramblerFunctions)
	{
		if (f.type == ScramblerType::Reverse) StringManipulation::reverse(v.s2);
		else if (f.type == ScramblerType::Slice) v.s2 = StringManipulation::slice(v.s2, f.parameter);
		else StringManipulation::swap(v.s2, f.parameter);
	}
	v.url += L"&" + v.sp + L"=";
	winrt::hstring encodedS2 = winrt::Windows::Foundation::Uri::EscapeComponent(v.s2);
	v.url += encodedS2.c_str();
}

void YoutubePlayerParser::Parse()
{
	FindScramblerFunction();
	ExtractScramblerFunctionInfo();
	FindScramblerAlgorithm();
	MatchAlgorithmWithFunction();
}
