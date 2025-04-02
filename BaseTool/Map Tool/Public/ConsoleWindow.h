#pragma once
#include "BasicWindow.h"



class ConsoleWindow : public BasicWindow
{
private:
	ConsoleWindow(void);
	virtual ~ConsoleWindow(void) = default;
	virtual void Free(void) override;
public:
	static ConsoleWindow* Create(void);

	virtual HRESULT CreateFrame(void) override;
	void Write(std::string stringLine);
	void WriteLine(std::string stringLine);

private:
	bool newLine;
	std::list<std::pair<bool, std::string>> line;
};
