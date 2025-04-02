#pragma once


extern HWND hwnd;
extern HINSTANCE instance;

std::string To_String(std::wstring targetString);
std::wstring To_WString(const std::string& targetString);


namespace Console
{
    void WriteLine(std::string stringLine);
    void Write(std::string stringLine);
}


class Base
{
protected:
	Base(void) = default;
	virtual ~Base(void) = default;
	virtual void Free(void) {}
public:
	static void Destroy(Base* pointer)
	{
		if (pointer == nullptr)
			return;

		pointer->Free();
		delete pointer;
	}

	template <typename T, typename std::enable_if<std::is_base_of<Base, T>::value, int>::type = 0>
	static void DestroyInstance(T*& pointer)
	{
		if (pointer == nullptr)
			return;

		Base::Destroy(pointer);
		pointer = nullptr;
	}

protected:

};