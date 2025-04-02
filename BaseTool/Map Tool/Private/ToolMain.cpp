#include "pch.h"
#include "ToolMain.h"

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;


// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();


ToolMain::ToolMain()
{
}

ToolMain::~ToolMain()
{
}

HRESULT ToolMain::Start(void)
{
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	backBufferColor = DirectX::XMFLOAT4((float)(clear_color.x * clear_color.w * 255.0f), (float)(clear_color.y * clear_color.w * 255.0f), (float)(clear_color.z * clear_color.w * 255.0f), (float)(clear_color.w * 255.0f));

	RECT rect;
	GetClientRect(hwnd, &rect);

	windowSizeX = static_cast<int>(rect.right - rect.left);
	windowSizeY = static_cast<int>(rect.bottom - rect.top);


	// Temp Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		return E_FAIL;
	}
	ImGuiStart();
	//ImGui_ImplWin32_EnableDpiAwareness();


	consoleWindow = ConsoleWindow::Create();
	mainViewWindow = nullptr;
	//mainViewWindow = CMainViewWindow::Create(renderTexture, renderSurface, dxDevice);


	//propertyWindow = CPropertyWindow::Create();
	//hierarchyWindow = CHierarchyWindow::Create(dxDevice);
	//resourceWindow = CResourceWindow::Create();
	//textureWindow = new CTextureWindow();
	//monitoringWindow = new CMonitoringWindow();
	//explorerWindow = new CFileExplorerWindow();

	//propertyWindow->LinkSelectedItem(hierarchyWindow->LinkSelectedItem());
	//mainViewWindow->LinkSelectedItem(hierarchyWindow->LinkSelectedItem());
	//mainViewWindow->LinkGameObjectList(hierarchyWindow->LinkGameObjectList());
	//mainViewWindow->LinkUIObjectList(hierarchyWindow->LinkUIObjectList());
	// Window Object Start


	return S_OK;
}

void ToolMain::FrameLoop(void)
{
	Update();
	//FixedUpdate();
	LateUpdate();

	if (FAILED(CheckWindow()))
		return;

	CreateFrames();

	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

	//Device()->Render(std::bind(&ToolMain::Render, this));
	Render();

	// Present
	//HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);


}

void ToolMain::Update(void)
{
	//Input->Update();
	//Time->Update();
	//mainViewWindow->Update();
	//propertyWindow->update();
	//
}

void ToolMain::FixedUpdate(void)
{
}

void ToolMain::LateUpdate(void)
{
}

void ToolMain::CreateFrames(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	MainMenuBar();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}


	consoleWindow->RenderGUI();
}

void ToolMain::Render(void)
{
	//mainViewWindow->RenderGUI();
	// Rendering
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

}

HRESULT ToolMain::CheckWindow(void)
{
	// Handle window being minimized or screen locked
	if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	{
		::Sleep(10);
		return E_FAIL;
	}
	g_SwapChainOccluded = false;
	return S_OK;


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
	// Handle window resize (we don't resize directly in the WM_SIZE handler)
	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}
	return S_OK;
}

#define WindowOnOff(Instance, InstanceName) if (ImGui::MenuItem(InstanceName)) Instance->SetActive(!Instance->IsActive());


void ToolMain::Free(void)
{
	// Cleanup
	Base::DestroyInstance(mainViewWindow);
	Base::DestroyInstance(consoleWindow);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
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
	if (!ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext))
		return E_FAIL;

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


void ToolMain::MainMenuBar(void)
{
	//ShortcutKey();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load", "Ctrl O"))
			{
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
			WindowOnOff(consoleWindow, "Console");
			//WindowOnOff(hierarchyWindow, "Hierarchy");
			//WindowOnOff(propertyWindow, "Property");
			//WindowOnOff(resourceWindow, "Resources");
			//WindowOnOff(textureWindow, "Textures");
			//WindowOnOff(monitoringWindow, "Monitor");
			//WindowOnOff(explorerWindow, "Explorer");
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




// Temp Render
bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;


	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
