#pragma once


extern HWND hwnd;
extern HINSTANCE instance;

//std::string To_String(std::wstring targetString);
//std::wstring To_WString(const std::string& targetString);


namespace Console
{
    void WriteLine(std::string stringLine);
    void Write(std::string stringLine);
}