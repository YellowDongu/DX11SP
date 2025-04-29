// Map Tool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "Map Tool.h"
#include "ToolMain.h"

#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE instance = nullptr;                                // 현재 인스턴스입니다.
HWND hwnd = nullptr;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND hWnd = nullptr;
HINSTANCE hInstance = nullptr;
ToolMain* mainApp = nullptr;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowSplashScreen(HINSTANCE hInstance);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAPTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    mainApp = ToolMain::Create();
    if (mainApp == nullptr)
    {
        ::DestroyWindow(hwnd);
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL));

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
            mainApp->FrameLoop();
    }

    Base::DestroyInstance(mainApp);
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPTOOL));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ZEROSIEVERTMAPTOOL);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInstance = instance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    hwnd = hWnd;

    ShowSplashScreen(hInstance);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
    case IDM_EXIT:
        ::DestroyWindow(hWnd);
        break;
    default:
        return ::DefWindowProcW(hWnd, message, wParam, lParam);
    }
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, message, wParam, lParam);
}






LRESULT CALLBACK SplashWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // GDI로 이미지 그리기
        HBITMAP hBitmap = (HBITMAP)LoadImage(nullptr, L"splash.bmp", IMAGE_BITMAP, 400, 300, LR_LOADFROMFILE);
        if (hBitmap)
        {
            HDC hMemDC = CreateCompatibleDC(hdc);
            SelectObject(hMemDC, hBitmap);
            BitBlt(hdc, 0, 0, 400, 300, hMemDC, 0, 0, SRCCOPY);
            DeleteDC(hMemDC);
            DeleteObject(hBitmap);
        }

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 스플래시 화면 표시 함수
void ShowSplashScreen(HINSTANCE hInstance)
{
    //mainApp = ToolMain::Create();
    //mainApp->Start();
    //return;
    //const wchar_t* splashClassName = L"SplashScreenClass";
    //
    //// 스플래시 창 클래스 등록
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, SplashWndProc, 0, 0, hInstance, nullptr, nullptr, nullptr, nullptr, splashClassName, nullptr };
    //RegisterClassEx(&wc);
    //
    //// 모니터 중앙에 창 생성
    //int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    //int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    //int splashWidth = 400, splashHeight = 300;
    //int x = (screenWidth - splashWidth) / 2;
    //int y = (screenHeight - splashHeight) / 2;
    //
    //HWND hSplashWnd = CreateWindowEx(0, splashClassName, L"Splash Screen", WS_POPUP | WS_VISIBLE, x, y, splashWidth, splashHeight, nullptr, nullptr, hInstance, nullptr);

    // 3초 대기
    //std::thread loadingThread(&ToolMain::Start, mainApp);
    //loadingThread.join();
    //std::this_thread::sleep_for(std::chrono::seconds(10));


    // 스플래시 창 제거
    //DestroyWindow(hSplashWnd);
    //UnregisterClass(splashClassName, hInstance);
}
















void Console::WriteLine(std::string stringLine)
{
    mainApp->consoleInstance()->WriteLine(stringLine);
}
void Console::Write(std::string stringLine)
{
    mainApp->consoleInstance()->Write(stringLine);
}
//void SetRenderState(void)
//{
//    mainApp->SetDeviceRenderState();
//}

