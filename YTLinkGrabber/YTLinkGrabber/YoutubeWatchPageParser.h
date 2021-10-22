#pragma once

class YoutubeWatchPageParser
{
public:
	std::wstring watchPageFile;
	std::wstring playerJsUrl;
	std::wstring adaptiveFormats;
	std::vector<Video> videos;
	YoutubeWatchPageParser(std::wstring file);
	void GetPlayerJsUrl();
	void GetAdaptiveFormats();
	void CreateVideosFromJson();
	void Parse();
};
