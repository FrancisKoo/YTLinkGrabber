#include "pch.h"
#include "Downloader.h"

Downloader::Downloader()
{
	winrt::Windows::Web::Http::Headers::HttpRequestHeaderCollection headers = httpClient.DefaultRequestHeaders();
	std::wstring header = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36";
	headers.UserAgent().ParseAdd(header);
}

winrt::hstring Downloader::SimpleDownloadToString(winrt::hstring& url)
{
	// reference: https://docs.microsoft.com/en-us/windows/uwp/networking/httpclient
	winrt::Windows::Foundation::Uri requestUri(url);

	// Send the GET request asynchronously, and retrieve the response as a string.
	winrt::Windows::Web::Http::HttpResponseMessage httpResponseMessage;
	winrt::hstring httpResponseBody;

	try
	{
		// Send the GET request.
		httpResponseMessage = httpClient.GetAsync(requestUri).get();
		httpResponseMessage.EnsureSuccessStatusCode();
		httpResponseBody = httpResponseMessage.Content().ReadAsStringAsync().get();
	}
	catch (winrt::hresult_error const& ex)
	{
		httpResponseBody = ex.message();
	}

	return httpResponseBody;
}
