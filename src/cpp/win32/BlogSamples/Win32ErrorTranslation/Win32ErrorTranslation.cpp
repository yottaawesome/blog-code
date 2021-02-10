#include <iostream>
#include <string>
#include <Windows.h>

std::wstring TranslateWin32ErrorCode(const DWORD errorCode) noexcept
{
    std::wstring errorMessage;
    void* ptrMsgBuf = nullptr;
    const DWORD flags =
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS;
    // See https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagew
    FormatMessageW(
        flags,              // dwFlags
        nullptr,            // lpSource
        errorCode,          // dwMessageId
        LANG_USER_DEFAULT,  // dwLanguageId
        (LPWSTR)&ptrMsgBuf, // lpBuffer
        0,                  // nSize
        nullptr             // Arguments
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
    // For a useful list of Win32 error codes, see: https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes
    std::wcout 
        << TranslateWin32ErrorCode(ERROR_FILE_NOT_FOUND) 
        << std::endl
        << TranslateWin32ErrorCode(ERROR_NOT_ENOUGH_MEMORY) 
        << std::endl
        << TranslateWin32ErrorCode(ERROR_INSUFFICIENT_BUFFER)
        << std::endl;

    return 0;
}
