#pragma once
//#include "CPropertyWindow.h"
//#include "CHierarchyWindow.h"
//#include "CResourceWindow.h"
#include "MainViewWindow.h"
#include "ConsoleWindow.h"
//#include "CTextureWindow.h"
//#include "CMonitoringWindow.h"
//#include "CFileExplorerWindow.h"


class ToolMain : public Base
{
private:
	ToolMain(void);
	virtual ~ToolMain(void);
	virtual void Free(void) override;
public:
	static ToolMain* Create(void);

    HRESULT Start(void);
	void FrameLoop(void);

	void Update(void);
	void FixedUpdate(void);
	void LateUpdate(void);
	void CreateFrames(void);
	void Render(void);

	HRESULT CheckWindow(void);

	ConsoleWindow* consoleInstance(void) { return consoleWindow; }
private:
	HRESULT ImGuiStart(void);
	void MainMenuBar(void);
	HRESULT DeviceRestart(void);

	bool show_demo_window = true;
	bool show_another_window = false;
	int windowSizeX, windowSizeY;

	RECT windowRect;
	std::wstring currentFilePath;
	DirectX::XMFLOAT4 backBufferColor;
	ImVec4 clear_color = {};
	//CPropertyWindow* propertyWindow;
	//CHierarchyWindow* hierarchyWindow;
	//CResourceWindow* resourceWindow;
	//CTextureWindow* textureWindow;
	//CMonitoringWindow* monitoringWindow;
	//CFileExplorerWindow* explorerWindow;
	MainViewWindow* mainViewWindow;
	ConsoleWindow* consoleWindow;
};
