#pragma once

class Downloader
{
public:
	Downloader();
	winrt::hstring SimpleDownloadToString(winrt::hstring& url);
private:
	winrt::Windows::Web::Http::HttpClient httpClient;
};
