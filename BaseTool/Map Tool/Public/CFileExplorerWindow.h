#pragma once
#include "CBasicWindow.h"


struct FileNode
{
	std::string folderName;
	std::vector<std::pair<std::string, std::string>> files;
	std::vector<FileNode> directories;
};


class CFileExplorerWindow : public BasicWindow
{
public:
	CFileExplorerWindow(void);
	~CFileExplorerWindow(void);

	HRESULT CreateFrame(void) override;
private:
	void showFiles(const FileNode& node);
	void ExploreDirectory(const std::wstring& directory, FileNode& node);
	void ShowFiles(const FileNode& node, bool lowMemoryMode = false);


	bool refresh;
	FileNode node;
	std::wstring rootDirectory;

	std::vector<std::wstring> files;
	std::vector<std::wstring> directories;

};