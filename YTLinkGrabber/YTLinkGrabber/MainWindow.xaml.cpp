#include "pch.h"
#include "Downloader.h"
#include "Video.h"
#include "YoutubeWatchPageParser.h"
#include "YoutubePlayerParser.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::YTLinkGrabber::implementation
{
	MainWindow::MainWindow()
	{
		InitializeComponent();
		Title(L"YouTube Video Download Link Grabber");
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::GetWorkDoneAsync(std::wstring url)
	{
		winrt::apartment_context ui_thread;

		co_await winrt::resume_background();

		Downloader d;
		hstring hUrl(url);
		std::wstring result;

		hstring response = d.SimpleDownloadToString(hUrl);

		YoutubeWatchPageParser p(response.c_str());
		bool bResults = true;
		try
		{
			p.Parse();
		}
		catch (...)
		{
			result = L"Parsing of YouTube response failed.";
			bResults = false;
		}
		if (!bResults || p.videos.size() == 0)
		{
			co_await ui_thread;

			txtInfo().Text(result);
			btnGetLinks().IsEnabled(true);
			co_return;
		}
		bool needToDecipher = !p.videos[0].signatureCipher.empty();

		if (needToDecipher)
		{
			hstring jsUrl(L"https://www.youtube.com" + p.playerJsUrl);
			response = d.SimpleDownloadToString(jsUrl);
			YoutubePlayerParser pp(response.c_str());
			try
			{
				pp.Parse();
			}
			catch (...)
			{
				result = L"Parsing of YouTube player script failed.";
				bResults = false;
			}
			if (!bResults)
			{
				co_await ui_thread;

				txtInfo().Text(result);
				btnGetLinks().IsEnabled(true);
				co_return;
			}
			for (Video& v : p.videos)
			{
				pp.ApplySignature(v);
			}
		}

		for (uint32_t i = 0; i < p.videos.size(); i++)
		{
			result.append(L"Media Type: ");
			result.append(p.videos[i].mimeType);
			result.append(L"\r\nVideo Quality: ");
			result.append(p.videos[i].qualityLabel);
			result.append(L"\r\nAudio Quality: ");
			result.append(p.videos[i].audioQuality);
			result.append(L"\r\nURL (copy link below and download with your favourite download manager ASAP):\r\n");
			result.append(p.videos[i].url);
			result.append(L"\r\n\r\n");
		}

		co_await ui_thread;

		txtInfo().Text(result);
		btnGetLinks().IsEnabled(true);
	}
}

void winrt::YTLinkGrabber::implementation::MainWindow::btnGetLinks_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
	std::wstring url = txtUrl().Text().c_str();
	if (url.find(L"https://www.youtube.com/watch?v=") != 0)
	{
		txtInfo().Text(L"Not a valid YouTube URL.");
		return;
	}
	btnGetLinks().IsEnabled(false);
	txtInfo().Text(L"Working...");
	GetWorkDoneAsync(url);
}
