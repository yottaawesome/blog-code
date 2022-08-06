#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <format>
#include <windows.h>

std::string ToString(const std::wstring& wstr)
{
	if (wstr.empty())
		return "";

	// Determine number of bytes needed to convert
	const DWORD bytesRequired = WideCharToMultiByte(
		CP_UTF8,										// CodePage
		0,												// dwFlags 
		&wstr[0],										// lpWideCharStr
		static_cast<int>(wstr.size()),					// cchWideChar 
		nullptr,										// lpMultiByteStr
		0,												// cbMultiByte
		nullptr,										// lpDefaultChar
		nullptr											// lpUsedDefaultChar
	);
	if (bytesRequired == 0)
		throw std::runtime_error("ConvertWStringToString(): WideCharToMultiByte() [1] failed");

	std::string strTo(bytesRequired / sizeof(char), '\0');
	// https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
	const DWORD status = WideCharToMultiByte(
		CP_UTF8,										// CodePage
		0,												// dwFlags 
		&wstr[0],										// lpWideCharStr
		static_cast<int>(wstr.size()),					// cchWideChar 
		&strTo[0],										// lpMultiByteStr
		static_cast<int>(strTo.size() * sizeof(char)),	// cbMultiByte
		nullptr,										// lpDefaultChar
		nullptr											// lpUsedDefaultChar
	);
	if (status == 0)
        throw std::runtime_error("ConvertWStringToString(): WideCharToMultiByte() [2] failed");

    return strTo;
}

struct Converter
{
	Converter(const wchar_t* v) {}
};

template <>
struct std::formatter<Converter> : std::formatter<std::string>
{
	auto format(Converter wstr, format_context& ctx)
	{
		return formatter<std::string>::format(std::format("{}", ""), ctx);
	}
};

template <>
struct std::formatter<std::wstring> : std::formatter<std::string>
{
    auto format(std::wstring wstr, format_context& ctx)
    {
        return formatter<std::string>::format(std::format("{}", ToString(wstr)), ctx);
    }
};



namespace std
{	
	/*std::string format(std::_Fmt_string<std::string> left, const wchar_t* val)
	{
		return "A";
	}*/
}

template <>
struct std::formatter<const wchar_t*> : std::formatter<std::string>
{
	auto format(const wchar_t* wstr, format_context& ctx)
	{
		return formatter<std::string>::format(std::format("{}", ToString(wstr)), ctx);
	}
};

template <>
struct std::formatter<const wchar_t(*)[2]> : std::formatter<std::string>
{
	auto format(auto wstr, format_context& ctx)
	{
		return formatter<std::string>::format(std::format("{}", ""), ctx);
	}
};

template <>
struct std::formatter<const wchar_t(&)[2]> : std::formatter<std::string>
{
	auto format(const auto wstr, format_context& ctx)
	{
		return formatter<std::string>::format(std::format("{}", ""), ctx);
	}
};

template<unsigned N>
struct FixedString {
	char buf[N + 1]{};
	constexpr FixedString(char const* s) {
		for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
	}
	constexpr operator char const* () const { return buf; }
};
template<unsigned N> FixedString(char const (&)[N])->FixedString<N - 1>;

template<FixedString T>
class Foo {
	static constexpr char const* Name = T;
public:
	void hello() const {};
};



template <size_t N>
struct H
{
	constexpr H(const char (&arg)[N])
	{

	}

	constexpr H(const wchar_t(&arg)[N])
	{

	}

	void Blah() {}
};
template<size_t N> H(char const (&)[N])->H<N>;

template<H T>
class Foo2 {

public:
	void hello() const { };
};

int main(int argc, char* args[]) try
{
	Foo<"Hello!"> foo;
	foo.hello();

	Foo2<"P"> p;
	//H a = "A";
	//H b = L"A";
	//H c = L"A";

    const std::wstring wstr = L"and I'm a wide string";
	const wchar_t* blah = L"and I'm also a wide string";
	//std::cout << std::format("I am a narrow string {}.\n", wstr);
	//std::cout << std::format("I am a narrow string {}.\n", blah);
	//std::cout << std::format("I am a narrow string {}.\n", "A", 1);
	//std::cout << std::format("I am a narrow string {} {}.\n", &L"A", (const wchar_t*)L"A");
	//std::cout << std::format("I am a narrow string {} {}.\n", L"A", (const wchar_t*)L"A");

    return 0;
}
catch (const std::exception& ex)
{
    std::cerr << std::format("Conversion failed with exception {}\n", ex.what());
}