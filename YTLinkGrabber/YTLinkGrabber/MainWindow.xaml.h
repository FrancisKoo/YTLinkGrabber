#pragma once

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "MainWindow.g.h"

#pragma pop_macro("GetCurrentTime")

namespace winrt::YTLinkGrabber::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
		MainWindow();
		winrt::Windows::Foundation::IAsyncAction GetWorkDoneAsync(std::wstring url);
		void btnGetLinks_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
	};
}

namespace winrt::YTLinkGrabber::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
