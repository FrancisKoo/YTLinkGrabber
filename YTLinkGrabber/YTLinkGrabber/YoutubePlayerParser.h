#pragma once

class YoutubePlayerParser
{
public:
	std::wstring playerFile;
	std::wstring scramblerFunction;
	std::wstring scramblerAlgorithm;
	std::vector<ScramblerFunction> scramblerFunctions;
	YoutubePlayerParser(std::wstring file);
	void FindScramblerFunction();
	void ExtractScramblerFunctionInfo();
	void FindScramblerAlgorithm();
	void MatchAlgorithmWithFunction();
	void ApplySignature(Video& v);
	void Parse();
};
