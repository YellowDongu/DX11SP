#include "GlobalArea.h"
#include "pch.h"


//std::string To_String(std::wstring targetString)
//{
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//    return converter.to_bytes(targetString);
//}
//
//std::wstring To_WString(const std::string& targetString)
//{
//    int wstrLength = MultiByteToWideChar(CP_UTF8, 0, targetString.c_str(), -1, nullptr, 0);
//    std::wstring wideString(wstrLength - 1, L'\0');
//    MultiByteToWideChar(CP_UTF8, 0, targetString.c_str(), -1, &wideString[0], wstrLength);
//    return wideString;
//}
//{
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    return converter.from_bytes(targetString);
//}
