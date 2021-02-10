#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

std::wstring GetErrorCodeString(const DWORD errorCode) noexcept
{
    std::wstring errorMessage;
    void* ptrMsgBuf = nullptr;
    const DWORD flags =
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS;
    FormatMessageW(
        flags,
        nullptr,
        errorCode,
        LANG_USER_DEFAULT,
        (LPTSTR)&ptrMsgBuf,
        0,
        nullptr
    );
    if (ptrMsgBuf == nullptr)
        return L"Failed to translate Win32 error code: " + std::to_wstring(errorCode);

    errorMessage = (LPWSTR)ptrMsgBuf;
    errorMessage += L" (error code: " + std::to_wstring(errorCode) + L")";
    LocalFree(ptrMsgBuf);

    return errorMessage;
}

int main(int argc, char* args[])
{
    std::wcout 
        << GetErrorCodeString(ERROR_FILE_NOT_FOUND) 
        << std::endl
        << GetErrorCodeString(ERROR_NOT_ENOUGH_MEMORY) 
        << std::endl
        << GetErrorCodeString(ERROR_INSUFFICIENT_BUFFER)
        << std::endl;

    return 0;
}
