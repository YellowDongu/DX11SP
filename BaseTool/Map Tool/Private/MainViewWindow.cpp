#include "pch.h"
#include "MainViewWindow.h"
#include "ToolCamera.h"
#include "DefaultModelShader.h"
#include "DefaultUIShader.h"
#include "DefaultCubeShader.h"


MainViewWindow::MainViewWindow(ID3D11Device* _device, ID3D11DeviceContext* _context) : BasicWindow(), device(_device), context(_context)
{
    device->AddRef();
    context->AddRef();
}

void MainViewWindow::Free(void)
{
    Base::DestroyInstance(camera);
    Base::DestroyInstance(lineDrawer);


    if (mainView != nullptr) { mainView->Release(); mainView = nullptr; }
    if (mainViewRenderTarget != nullptr) { mainViewRenderTarget->Release(); mainViewRenderTarget = nullptr; }
    if (backBufferTexture != nullptr) { backBufferTexture->Release(); backBufferTexture = nullptr; }
    if (renderTexture != nullptr) { renderTexture->Release(); renderTexture = nullptr;  }
    if (renderRTV != nullptr) { renderRTV->Release(); renderRTV = nullptr; }
    if (renderSRV != nullptr) { renderSRV->Release(); renderSRV = nullptr; }

    if (device != nullptr)
    {
        device->Release();
        device = nullptr;
    }
    if (context != nullptr)
    {
        context->Release();
        context = nullptr;
    }
    viewPort = nullptr;
    dxDevice = nullptr;
}
Engine::Shader* shader;
HRESULT MainViewWindow::Start(void)
{
    if (dxDevice == nullptr)
        return E_FAIL;

    viewPort = &dxDevice->ViewPortInfomation();
    //if(FAILED(dxDevice->CreateImGuiView(mainView)))
    //    return E_FAIL;
    //if (FAILED(CreateView(dxDevice->GetDXSwapChain(), mainView, backBufferTexture)))
    //    return E_FAIL;
    //CreateView();
    AddShader(L"DefaultUIShader", Engine::DefaultUIShader::Create(device, context));
    AddShader(L"DefaultCubeShader", Engine::DefaultCubeShader::Create(device, context));
    AddShader(L"DefaultModelShader", shader = Engine::DefaultModelShader::Create(device, context));
    camera = ToolCamera::Create(device, context);
    if (camera == nullptr)
        return E_FAIL;

    lineDrawer = Engine::LineDrawer::Create(device, context);
    return S_OK;
}

void MainViewWindow::Update()
{
    camera->Update();
}

void MainViewWindow::Render()
{
    lineDrawer->AddPosition(Vector3{FLT_MIN, 0.0f, 0.0f});
    lineDrawer->AddPosition(Vector3{FLT_MAX, 0.0f, 0.0f});
    lineDrawer->SetColor(float4(1.0f,0.0f,0.0f,1.0f));
    lineDrawer->Render();
    lineDrawer->PopFrontPosition();
    lineDrawer->PopFrontPosition();
    lineDrawer->AddPosition(Vector3{ 0.0f, FLT_MIN, 0.0f});
    lineDrawer->AddPosition(Vector3{ 0.0f, FLT_MAX, 0.0f});
    lineDrawer->SetColor(float4(0.0f,1.0f,0.0f,1.0f));
    lineDrawer->Render();
    lineDrawer->PopFrontPosition();
    lineDrawer->PopFrontPosition();
    lineDrawer->AddPosition(Vector3{ 0.0f, 0.0f, FLT_MIN });
    lineDrawer->AddPosition(Vector3{ 0.0f, 0.0f, FLT_MAX });
    lineDrawer->SetColor(float4(0.0f,0.0f,1.0f,1.0f));
    lineDrawer->Render();
    lineDrawer->PopFrontPosition();
    lineDrawer->PopFrontPosition();
}

HRESULT MainViewWindow::CreateFrame()
{
    HRESULT result = E_FAIL;

    //dxDevice->Render(std::bind(&MainViewWindow::Render, this));

    ImGui::Begin("View");
    //ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    mouseHovering = ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
    focused = ImGui::IsWindowFocused();
    if (mainView)
    {
        ImVec2 outputSize = windowSize = ImGui::GetWindowSize();
        outputSize.x -= 15.0f;
        outputSize.y -= 35.0f;
        ImGui::Image((ImTextureID)mainView, outputSize);
        result = S_OK;
    }
    ImGui::End();
    camera->Render();
    return result;
}

HRESULT MainViewWindow::CreateView(IDXGISwapChain* swapChain, ID3D11ShaderResourceView*& mainView, ID3D11Texture2D*& backBufferTexture)
{
    HRESULT result = S_OK;
    if (swapChain == nullptr)
        return E_FAIL;

    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
        return E_FAIL;
    
    if (FAILED(device->CreateShaderResourceView(backBufferTexture, nullptr, &mainView)))
        result = E_FAIL;
    return result;
}

MainViewWindow* MainViewWindow::Create(Engine::DXDevice* deviceInstance)
{
    MainViewWindow* newInstance = new MainViewWindow(deviceInstance->GetDevice(), deviceInstance->GetDeviceContext());

    newInstance->dxDevice = deviceInstance;
    if (FAILED(newInstance->Start()))
    {
        Base::DestroyInstance(newInstance);
        return nullptr;
    }

    return newInstance;
}
/*
void CMainViewWindow::DrawAxes(IDirect3DDevice9* device)
{
    D3DXMATRIX worldMatrix;
    D3DXMatrixIdentity(&worldMatrix);
    device->SetTransform(D3DTS_WORLD, &worldMatrix);

    if (!axisBuffer)
    {
        std::vector<Vertex> vertices = {
            // X red
            { D3DXVECTOR3(-65535.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) },
            { D3DXVECTOR3( 65535.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) },

            // Y green
            { D3DXVECTOR3(0.0f, -65535.0f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) },
            { D3DXVECTOR3(0.0f,  65535.0f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) },

            // Z blue
            { D3DXVECTOR3(0.0f, 0.0f, -65535.0f), D3DCOLOR_XRGB(0, 0, 255) },
            { D3DXVECTOR3(0.0f, 0.0f,  65535.0f), D3DCOLOR_XRGB(0, 0, 255) },
        };

        device->CreateVertexBuffer((UINT)vertices.size() * (UINT)sizeof(Vertex), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &axisBuffer, NULL);

        void* pVertices;
        axisBuffer->Lock(0, 0, (void**)&pVertices, 0);
        memcpy(pVertices, vertices.data(), vertices.size() * sizeof(Vertex));
        axisBuffer->Unlock();
    }



    device->SetStreamSource(0, axisBuffer, 0, sizeof(Vertex));
    device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    device->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}

void CMainViewWindow::DrawGrid(IDirect3DDevice9* device)
{
    if (!device)
        return;

    D3DXMATRIX viewMatrix, invViewMatrix;
    D3DXVECTOR3 cameraPos;
    device->GetTransform(D3DTS_VIEW, &viewMatrix);
    D3DXMatrixInverse(&invViewMatrix, NULL, &viewMatrix);

    cameraPos.x = (float)((int)invViewMatrix._41 / 10) * 10.0f;
    cameraPos.y = 0.0f;
    cameraPos.z = (float)((int)invViewMatrix._43 / 10) * 10.0f;
    D3DXMATRIX worldMatrix;
    D3DXMatrixTranslation(&worldMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
    device->SetTransform(D3DTS_WORLD, &worldMatrix);

    if (!gridBuffer)
    {
        CreateGrid(device, 10.0f, 10.0f);
    }



    device->SetStreamSource(0, gridBuffer, 0, sizeof(Vertex));
    device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    //device->DrawPrimitive(D3DPT_LINELIST, 0, vertices.size() / 2);
    device->DrawPrimitive(D3DPT_LINELIST, 0, (UINT)vertices.size() / 2);

}

void CMainViewWindow::CreateGrid(IDirect3DDevice9* device, float gridSize, float gridSpacing)
{
    float min = -gridSpacing * (gridSize / 2.0f);
    float max = gridSpacing * (gridSize / 2.0f);

    for (int x = -(int)gridSize / 2; x <= (int)gridSize / 2; x++)
    {
        vertices.push_back({ D3DXVECTOR3((float)x * gridSpacing, 0.0f, min), D3DCOLOR_XRGB(200, 200, 200) });
        vertices.push_back({ D3DXVECTOR3((float)x * gridSpacing, 0.0f, max), D3DCOLOR_XRGB(200, 200, 200) });

    }
    for (int y = -(int)gridSize / 2; y <= (int)gridSize / 2; y++)
    {

        vertices.push_back({ D3DXVECTOR3(min, 0.0f, (float)y * gridSpacing), D3DCOLOR_XRGB(200, 200, 200) });
        vertices.push_back({ D3DXVECTOR3(max, 0.0f, (float)y * gridSpacing), D3DCOLOR_XRGB(200, 200, 200) });
    }

    device->CreateVertexBuffer((UINT)vertices.size() * (UINT)sizeof(Vertex), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &gridBuffer, NULL);

    void* pVertices;
    gridBuffer->Lock(0, 0, (void**)&pVertices, 0);
    memcpy(pVertices, vertices.data(), vertices.size() * sizeof(Vertex));
    gridBuffer->Unlock();
}

void CMainViewWindow::raycastViewSpace(const LPDIRECT3DDEVICE9& device, vec2& mousePosition, vec3& rayDirection)
{
    D3DVIEWPORT9 viewport;
    ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));
    if (FAILED(device->GetViewport(&viewport)))
        return;

    D3DXMATRIXA16 projectionMatrix, projectionMatrixInverse;
    ZeroMemory(&projectionMatrix, sizeof(D3DXMATRIXA16));
    if (FAILED(device->GetTransform(D3DTS_PROJECTION, &projectionMatrix)))
        return;

    D3DXMatrixInverse(&projectionMatrixInverse, 0, &projectionMatrix);

    rayDirection.x = ((2.0f * mousePosition.x) / viewport.Width - 1.0f) * projectionMatrixInverse._11;
    rayDirection.y = ((-2.0f * mousePosition.y) / viewport.Height + 1.0f) * projectionMatrixInverse._22;
    rayDirection.z = projectionMatrixInverse._43;
}

void CMainViewWindow::raycastWorldSpace(const LPDIRECT3DDEVICE9& device, vec2& mousePosition, vec3& rayOrigin, vec3& rayDirection)
{
    raycastViewSpace(device, mousePosition, rayDirection);
    D3DXMATRIXA16 viewMatrix, viewMatrixInverse;

    if (FAILED(device->GetTransform(D3DTS_VIEW, &viewMatrix)))
        return;

    D3DXMatrixInverse(&viewMatrixInverse, 0, &viewMatrix);

    rayOrigin = { viewMatrixInverse._41, viewMatrixInverse._42, viewMatrixInverse._43 };
    D3DXVec3TransformNormal(&rayDirection, &rayDirection, &viewMatrixInverse);
    D3DXVec3Normalize(&rayDirection, &rayDirection);
}

*/