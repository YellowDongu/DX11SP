#pragma once
#include "Base.h"
#include "PropertyWindow.h"
#include "HierarchyWindow.h"
//#include "CResourceWindow.h"
#include "MainViewWindow.h"
#include "ConsoleWindow.h"
//#include "CTextureWindow.h"
#include "MonitoringWindow.h"
//#include "CFileExplorerWindow.h"
#include "NaviMeshCreatorWindow.h"


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
	HRESULT CreateWindows(void);
	void MainMenuBar(void);
	HRESULT DeviceRestart(void);


	bool show_demo_window{ true };
	bool show_another_window{ false };
	int windowSizeX{ 0 }, windowSizeY{ 0 };

	RECT windowRect{};
	std::wstring currentFilePath{};
	DirectX::XMFLOAT4 backBufferColor{};
	ImVec4 clear_color{};
	ObjectManager* objectManager{ nullptr };

	//CResourceWindow* resourceWindow;
	//CTextureWindow* textureWindow;
	//CFileExplorerWindow* explorerWindow;
	PropertyWindow* propertyWindow{ nullptr };
	HierarchyWindow* hierarchyWindow{ nullptr };
	MonitoringWindow* monitoringWindow{nullptr};
	MainViewWindow* mainViewWindow{nullptr};
	ConsoleWindow* consoleWindow{nullptr};
	NaviMeshCreatorWindow* naviWindow{nullptr};
};
