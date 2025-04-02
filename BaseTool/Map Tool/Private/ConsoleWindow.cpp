#include "pch.h"
#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow() : newLine(true), line{}
{
}


HRESULT ConsoleWindow::CreateFrame(void)
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Clear##Console"))
		{
			line.clear();
		}

		ImGui::EndMenuBar();
	}
	for (auto& str : line)
	{
		ImGui::Text(str.second.c_str());
	}

	if (newLine)
	{
		ImGui::SetScrollHereY(1.0f);
		newLine = false;
	}
	ImGui::End();

	return S_OK;
}

void ConsoleWindow::Write(std::string stringLine)
{
	std::vector<std::string> lines;
	size_t start = 0;
	size_t end = 0;

	do
	{
		end = stringLine.find('\n', start);
		if (end == std::string::npos)
			lines.push_back("Log : " + stringLine.substr(start));
		else
		{
			lines.push_back("Log : " + stringLine.substr(start, end - start));
			start = end + 1;
		}
	}
	while (end != std::string::npos);

	lines.push_back(stringLine.substr(start));

	for (auto& string : lines)
	{
		line.push_back({ false, "Log : " + string });
	}
	newLine = true;
}

void ConsoleWindow::WriteLine(std::string stringLine)
{
	line.push_back({ false, "Log : " + stringLine });
	newLine = true;
}

void ConsoleWindow::Free(void)
{
	line.clear();
}

ConsoleWindow* ConsoleWindow::Create(void)
{
	ConsoleWindow* newInstance = new ConsoleWindow();
	return newInstance;
}

