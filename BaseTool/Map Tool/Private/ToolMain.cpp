#include "pch.h"
#include "ToolMain.h"
#include "ObjectManager.h"
#include "JsonIO.h"
#include "ToolCamera.h"
#include "DefaultUIShader.h"
ToolMain::ToolMain()
{
}

ToolMain::~ToolMain()
{
}

HRESULT ToolMain::Start(void)
{
	Engine::DeviceInfo deviceInfo;
	RECT rect;

	ZeroMemory(&deviceInfo, sizeof(Engine::DeviceInfo));
	GetClientRect(hwnd, &rect);

	deviceInfo.hWnd = hwnd;
	deviceInfo.hInstance = instance;
	deviceInfo.isWindowed = true;

	windowSizeX = static_cast<int>(rect.right - rect.left);
	windowSizeY = static_cast<int>(rect.bottom - rect.top);

	deviceInfo.viewpoirtLength = windowSizeY;
	deviceInfo.viewpoirtWidth = windowSizeX;
	deviceInfo.shaderOutput = true;

	Engine::GameInstance* gameInstance = Engine::GameInstance::Create(deviceInfo);
	gameInstance->EntireInitialize();
	gameInstance->Time()->setTargetFPS(144);
	gameInstance->Device()->SetBackBufferColor({ 153.0f / 256.0f * 0.8f, 217.0f / 256.0f * 0.6f, 234.0f / 256.0f * 0.8f, 1.0f });
	::SetModelVerticeSaveMode(true);
	ImGuiStart();
	Engine::DefaultUIShader* uiShader = Engine::DefaultUIShader::Create(DxDevice(), DxDeviceContext());
	::AddShader(uiShader->shaderFile, uiShader);
	::EngineInstance()->RenderManager()->ChangeUIShader(uiShader);
	//ImGui_ImplWin32_EnableDpiAwareness();
	objectManager = ObjectManager::Create(DxDevice(), DxDeviceContext());
	CreateWindows();
	return S_OK;
}

void ToolMain::FrameLoop(void)
{
	//if (FAILED(CheckWindow()))
	//	return;
	EngineInstance()->TimeUpdate();
	EngineInstance()->InTimeUpdate();
	Update();
	LateUpdate();

	//if (FAILED(CheckWindow()))
	//	return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	//ID3D11RenderTargetView* const& targetView = (ID3D11RenderTargetView* const)(Device()->GetTargetView());
	//DxDeviceContext()->OMSetRenderTargets(1, &targetView, nullptr);
	//mainViewWindow->SetRenderTarget();
	Device()->Render(std::bind(&ToolMain::Render, this));
}

void ToolMain::Update(void)
{
	objectManager->Update();
	//Input->Update();
	//Time->Update();
	mainViewWindow->Update();
	//propertyWindow->update();
	naviWindow->Update();
}

void ToolMain::FixedUpdate(void)
{
	objectManager->FixedUpdate();
}

void ToolMain::LateUpdate(void)
{
	objectManager->LateUpdate();
}

void ToolMain::CreateFrames(void)
{
	// Start the Dear ImGui frame
	MainMenuBar();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	consoleWindow->RenderGUI();
	monitoringWindow->RenderGUI();
	hierarchyWindow->RenderGUI();
	propertyWindow->RenderGUI();
	naviWindow->RenderGUI();
}

void ToolMain::Render(void)
{
	SetShader(L"DefaultModelShader");
	Matrix identity;
	DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
	SetWorldMatrix(identity);
	
	mainViewWindow->Render();
	EngineInstance()->RenderManager()->Render();
	naviWindow->Render();
	
	//mainViewWindow->SetRenderTarget();
	mainViewWindow->CreateFrame();
	CreateFrames();
	// 5. ImGui ·»´õ¸µ
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		if (hwnd == 0)
			return;
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	//mainViewWindow->RenderGUI();
	// 
	// Rendering

}

HRESULT ToolMain::CheckWindow(void)
{
	// Handle window being minimized or screen locked
	//if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	//{
	//	::Sleep(10);
	//	return E_FAIL;
	//}
	GetClientRect(hwnd, &windowRect);
	int x = (int)(windowRect.right - windowRect.left);
	int y = (int)(windowRect.bottom - windowRect.top);
	if (windowSizeX != x || windowSizeY != y)
	{
		windowSizeX = x;
		windowSizeY = y;
		//if (FAILED(DeviceRestart()))
		//{
		//	windowSizeX = 0;
		//	windowSizeY = 0;
		//}
		//return;
	}

	return S_OK;
}

#define WindowOnOff(Instance, InstanceName) if (ImGui::MenuItem(InstanceName)) Instance->SetActive(!Instance->IsActive());


void ToolMain::Free(void)
{
	// Cleanup
	Base::DestroyInstance(hierarchyWindow);
	Base::DestroyInstance(propertyWindow);
	Base::DestroyInstance(monitoringWindow);
	Base::DestroyInstance(mainViewWindow);
	Base::DestroyInstance(consoleWindow);
	Base::DestroyInstance(naviWindow);
	
	Base::DestroyInstance(objectManager);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyGameInstance();
}

ToolMain* ToolMain::Create(void)
{
	ToolMain* newInstance = new ToolMain();
	if (FAILED(newInstance->Start()))
	{
		newInstance->Free();
		delete newInstance;
		return nullptr;
	}
	return newInstance;
}


HRESULT ToolMain::ImGuiStart(void)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	// 
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(hwnd))
		return E_FAIL;
	if (!ImGui_ImplDX11_Init(DxDevice(), DxDeviceContext()))
		return E_FAIL;
	//if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext))
	//	return E_FAIL;

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);
	return S_OK;
}

HRESULT ToolMain::CreateWindows(void)
{
	consoleWindow = ConsoleWindow::Create();
	if (consoleWindow == nullptr)
		return E_FAIL;
	mainViewWindow = MainViewWindow::Create(Device());
	if (mainViewWindow == nullptr)
		return E_FAIL;
	monitoringWindow = MonitoringWindow::Create();
	monitoringWindow->LinkCamera(mainViewWindow->LinkCamera());
	//if (monitoringWindow == nullptr)
	//	return E_FAIL;
	propertyWindow = PropertyWindow::Create(objectManager);
	hierarchyWindow = HierarchyWindow::Create(objectManager);
	naviWindow = NaviMeshCreatorWindow::Create(objectManager);
	//resourceWindow = CResourceWindow::Create();
	//textureWindow = new CTextureWindow();
	//explorerWindow = new CFileExplorerWindow();

	//propertyWindow->LinkSelectedItem(hierarchyWindow->LinkSelectedItem());
	//mainViewWindow->LinkSelectedItem(hierarchyWindow->LinkSelectedItem());
	//mainViewWindow->LinkGameObjectList(hierarchyWindow->LinkGameObjectList());
	//mainViewWindow->LinkUIObjectList(hierarchyWindow->LinkUIObjectList());
	return S_OK;
}


void ToolMain::MainMenuBar(void)
{
	//ShortcutKey();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load", "Ctrl O"))
			{
				CJsonIO::Load(objectManager);
				//std::wstring path = CJsonIO::selectLoadFilePath();
				//Console::WriteLine(To_String(path));
				//hierarchyWindow->LinkSelectedItem() = nullptr;
				//Console::WriteLine(CJsonIO::LoadJson());
				//if (lstrlenW(path.data()) != 0)
				//{
				//	if(SUCCEEDED(CJsonIO::Load_Scene(hierarchyWindow->LinkGameObjectList(), hierarchyWindow->LinkUIObjectList(), path)))
				//		currentFilePath = path;
				//}
			}
			if (ImGui::MenuItem("overrideLoad"))
			{
				//std::wstring path = CJsonIO::selectLoadFilePath();
				//Console::WriteLine(To_String(path));
				//hierarchyWindow->LinkSelectedItem() = nullptr;
				//Console::WriteLine(CJsonIO::LoadJson());
				//if (lstrlenW(path.data()) != 0)
				//{
				//	if (SUCCEEDED(CJsonIO::OverrideLoad_Scene(hierarchyWindow->LinkGameObjectList(), hierarchyWindow->LinkUIObjectList(), path)))
				//		currentFilePath = path;
				//}
			}
			if (ImGui::MenuItem("Save", "Ctrl S"))
			{
				CJsonIO::Save(objectManager);
				//std::wstring path = CJsonIO::selectSaveFilePath();
				//Console::WriteLine(To_String(path));
				//if (lstrlenW(path.data()) != 0)
				//{
				//	CJsonIO::Save_Scene(hierarchyWindow->LinkGameObjectList(), hierarchyWindow->LinkUIObjectList(), path);
				//	currentFilePath = path;
				//}
			}
			if (ImGui::MenuItem("SaveNew", "Ctrl Shift S"))
			{
				CJsonIO::Load(objectManager);
				//std::wstring path = CJsonIO::selectSaveFilePath();
				//Console::WriteLine(To_String(path));
				//if (lstrlenW(path.data()) != 0)
				//{
				//	CJsonIO::Save_Scene(hierarchyWindow->LinkGameObjectList(), hierarchyWindow->LinkUIObjectList(), path);
				//	currentFilePath = path;
				//}
			}
			if (ImGui::MenuItem("Exit")) { ::PostQuitMessage(0); }
			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("Options"))
		{
			//if (ImGui::BeginMenu("RenderState"))
			//{
			//	if (ImGui::BeginMenu("CullMode"))
			//	{
			//		if (ImGui::MenuItem("Clockwise")) { dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); cullMode = D3DCULL_CW; }
			//		if (ImGui::MenuItem("CounterClockwise")) { dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); cullMode = D3DCULL_CCW; }
			//		if (ImGui::MenuItem("None")) { dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); cullMode = D3DCULL_NONE; }
			//		ImGui::EndMenu();
			//	}
			//	if (ImGui::BeginMenu("FillMode"))
			//	{
			//		if (ImGui::MenuItem("Wireframe")) { dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); fillType = D3DFILL_WIREFRAME; }
			//		if (ImGui::MenuItem("Point")) { dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT); fillType = D3DFILL_POINT; }
			//		if (ImGui::MenuItem("None")) { dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); fillType = D3DFILL_SOLID; }
			//		ImGui::EndMenu();
			//	}
			//	if (ImGui::BeginMenu("Light"))
			//	{
			//		if (ImGui::MenuItem("Enable")) { dxDevice->SetRenderState(D3DRS_LIGHTING, TRUE); light = true; }
			//		if (ImGui::MenuItem("Disable")) { dxDevice->SetRenderState(D3DRS_LIGHTING, FALSE); light = false; }
			//		ImGui::EndMenu();
			//	}
			//	if (ImGui::BeginMenu("Z Axis"))
			//	{
			//		if (ImGui::MenuItem("ZEnable")) { dxDevice->SetRenderState(D3DRS_ZENABLE, TRUE); zEnable = true; }
			//		if (ImGui::MenuItem("ZDisable")) { dxDevice->SetRenderState(D3DRS_ZENABLE, FALSE); zEnable = false; }
			//		if (ImGui::MenuItem("ZWriteEnable")) { dxDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); zWrite = true; }
			//		if (ImGui::MenuItem("ZWriteDisable")) { dxDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); zWrite = false; }
			//
			//		ImGui::EndMenu();
			//	}
			//
			//	ImGui::EndMenu();
			//}
			//if (ImGui::MenuItem("Preview UI")) { mainViewWindow->UIPreviewActive(!mainViewWindow->UIPreviewActive()); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			//WindowOnOff(explorerWindow, "Explorer");
			WindowOnOff(propertyWindow, "PropertyWindow");
			WindowOnOff(hierarchyWindow, "HierarchyWindow");
			WindowOnOff(monitoringWindow, "MonitoringWindow");
			WindowOnOff(mainViewWindow, "MainViewWindow");
			WindowOnOff(consoleWindow, "ConsoleWindow");
			WindowOnOff(naviWindow, "NavMeshWindow");
			if (ImGui::MenuItem("Demo")) { show_demo_window = !show_demo_window; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}



}

HRESULT ToolMain::DeviceRestart(void)
{
	return S_OK;
	//if (CGraphicDev::GetInstance()->Get_GrphicDev() == nullptr)
	//    return S_OK;
	//if (windowSizeX == 0 || windowSizeY == 0)
	//	return S_OK;
	//
	//
	//if (renderTexture)
	//{
	//	renderTexture->Release();
	//	renderTexture = nullptr;
	//}
	//if (renderSurface)
	//{
	//	renderSurface->Release();
	//	renderSurface = nullptr;
	//}
	//
	//ImGui_ImplDX9_InvalidateDeviceObjects();
	//
	//Engine::WINMODE mode = Engine::WINMODE::MODE_WIN;
	//HRESULT result = CGraphicDev::GetInstance()->change_Window(mode, (uint)windowSizeX, (uint)windowSizeY);
	//
	//if (FAILED(SetSurface()))
	//	return E_FAIL;
	//
	//// Device Setting
	//mainViewWindow->renderSurface = renderSurface;
	//mainViewWindow->renderTexture = renderTexture;
	//if (ImGui_ImplDX9_CreateDeviceObjects())
	//	return S_OK;
	//
	//return E_FAIL;
}





