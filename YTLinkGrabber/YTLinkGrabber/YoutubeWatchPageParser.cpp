#include "pch.h"
#include "Video.h"
#include "StringManipulation.h"
#include "YoutubeWatchPageParser.h"

YoutubeWatchPageParser::YoutubeWatchPageParser(std::wstring file)
{
	if (file.empty()) throw std::runtime_error("parameter file is empty.");
	watchPageFile = file;
}

void YoutubeWatchPageParser::GetPlayerJsUrl()
{
	if (watchPageFile.empty()) throw std::runtime_error("member watchPageFile is empty.");
	std::wregex pattern(L"\"PLAYER_JS_URL\":\"(.+?)\"");
	std::wsmatch match;
	if (regex_search(watchPageFile, match, pattern))
	{
		playerJsUrl = match.str(1);
	}
}

void YoutubeWatchPageParser::GetAdaptiveFormats()
{
	if (watchPageFile.empty()) throw std::runtime_error("member watchPageFile is empty.");
	std::wregex pattern(L"\"adaptiveFormats\":(\\[[\\s\\S]*?\\])");
	std::wsmatch match;
	if (regex_search(watchPageFile, match, pattern))
	{
		adaptiveFormats = match.str(1);
	}
}

void YoutubeWatchPageParser::CreateVideosFromJson()
{
	using namespace winrt::Windows::Data::Json;
	if (adaptiveFormats.empty()) throw std::runtime_error("member adaptiveFormats is empty.");

	JsonValue v(nullptr);
	if (!JsonValue::TryParse(adaptiveFormats, v)) return;
	if (v.ValueType() != JsonValueType::Array) return;

	JsonArray arr = v.GetArray();
	for (uint32_t i = 0; i < arr.Size(); i++)
	{
		IJsonValue element = arr.GetAt(i);
		if (element.ValueType() == JsonValueType::Object)
		{
			JsonObject obj = element.GetObject();
			Video vi;
			vi.url = obj.GetNamedString(L"url", L"");
			vi.signatureCipher = obj.GetNamedString(L"signatureCipher", L"");
			vi.qualityLabel = obj.GetNamedString(L"qualityLabel", L"");
			vi.audioQuality = obj.GetNamedString(L"audioQuality", L"");
			vi.mimeType = obj.GetNamedString(L"mimeType", L"");
			if (vi.url.empty() && !vi.signatureCipher.empty())
			{
				vi.s = StringManipulation::GetQueryParam(vi.signatureCipher, L"s");
				vi.sp = StringManipulation::GetQueryParam(vi.signatureCipher, L"sp");
				vi.url = StringManipulation::GetQueryParam(vi.signatureCipher, L"url");
			}
			videos.push_back(vi);
		}
	}
}

void YoutubeWatchPageParser::Parse()
{
	GetPlayerJsUrl();
	GetAdaptiveFormats();
	CreateVideosFromJson();
}
