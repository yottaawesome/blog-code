#include <iostream>
#include <stdexcept>
#include <string>
#include <format>
#include <windows.h>

std::string ToString(const std::wstring& wstr)
{
    if (wstr.empty())
        return "";

    std::string strTo(wstr.size(), '\0');
    // https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
    const DWORD status = WideCharToMultiByte(
        CP_UTF8,			// CodePage
        0,					// dwFlags 
        wstr.c_str(),		// lpWideCharStr
        (int)wstr.size(),	// cchWideChar 
        &strTo[0],			// lpMultiByteStr
        (int)strTo.size(),	// cbMultiByte
        nullptr,			// lpDefaultChar
        nullptr				// lpUsedDefaultChar
    );
    if (status == 0)
        throw std::runtime_error("ConvertWStringToString(): WideCharToMultiByte() failed");

    return strTo;
}

template <>
struct std::formatter<std::wstring> : std::formatter<std::string>
{
    auto format(std::wstring wstr, format_context& ctx)
    {
        return formatter<string>::format(std::format("{}", ToString(wstr)), ctx);
    }
};

int main(int argc, char* args[]) try
{
    const std::wstring wstr = L"and I am a wide string";
    std::cout << std::format("I am a narrow string {}.\n", wstr);

    return 0;
}
catch (const std::exception& ex)
{
    std::cerr << std::format("Conversion failed with exception {}\n", ex.what());
}