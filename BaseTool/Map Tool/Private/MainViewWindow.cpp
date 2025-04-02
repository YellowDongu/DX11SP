#include "pch.h"
#include "MainViewWindow.h"

MainViewWindow::MainViewWindow(ID3D11Device* _device, ID3D11DeviceContext* _context) : BasicWindow(), device(_device), context(_context)
{
}

void MainViewWindow::Free(void)
{
}

HRESULT MainViewWindow::Start(void)
{
    return S_OK;
    return E_FAIL;
}

void MainViewWindow::Update()
{
}

void MainViewWindow::Render()
{
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    context->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
    context->ClearRenderTargetView(mainRenderTargetView, clear_color_with_alpha);
}

HRESULT MainViewWindow::CreateFrame()
{
    HRESULT result = E_FAIL;
    ImGui::Begin("View");
    if (mainView)
    {
        //ImGui::Begin("View", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        mouseHovering = ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
        focused = ImGui::IsWindowFocused();
        ImVec2 outputSize = windowSize = ImGui::GetWindowSize();
        outputSize.x -= 15.0f;
        outputSize.y -= 35.0f;
        ImGui::Image((ImTextureID)mainView, outputSize);
        result = S_OK;
    }
    ImGui::End();
    return result;
}


MainViewWindow* MainViewWindow::Create(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* shaderResourceView, ID3D11RenderTargetView* mainRenderTargetView)
{
    MainViewWindow* newInstance = new MainViewWindow(device, context);

    newInstance->mainView = shaderResourceView;
    newInstance->mainRenderTargetView = mainRenderTargetView;

    if (FAILED(newInstance->Start()))
    {
        Base::DestroyInstance(newInstance);
        return nullptr;
    }

    newInstance->mainView->AddRef();
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
            // X축 (빨간색)
            { D3DXVECTOR3(-65535.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) },
            { D3DXVECTOR3( 65535.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) },

            // Y축 (초록색)
            { D3DXVECTOR3(0.0f, -65535.0f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) },
            { D3DXVECTOR3(0.0f,  65535.0f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) },

            // Z축 (파란색)
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