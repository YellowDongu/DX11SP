#pragma once
#include "BasicWindow.h"
#include "LineDrawer.h" 

class MainViewWindow : public BasicWindow
{
private:
	MainViewWindow(void) = delete;
	MainViewWindow(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~MainViewWindow(void) = default;
	virtual void Free(void) override;
public:
	static MainViewWindow* Create(Engine::DXDevice* deviceInstance);

	HRESULT Start(void);
	void Update(void);
	void Render(void);
	virtual HRESULT CreateFrame(void) override;

	HRESULT CreateView(IDXGISwapChain* swapChain, ID3D11ShaderResourceView*& mainView, ID3D11Texture2D*& backBufferTexture);

	void SetRenderTarget(void) {
		context->OMSetRenderTargets(1, &renderRTV, nullptr);
		context->ClearRenderTargetView(renderRTV, (FLOAT*)&color);
	}
	HRESULT CreateView()
	{

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = 800;  // 원하는 크기
		texDesc.Height = 600;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		if (FAILED(device->CreateTexture2D(&texDesc, nullptr, &renderTexture)))
			return E_FAIL;
		if(FAILED(device->CreateRenderTargetView(renderTexture, nullptr, &renderRTV)))
		   return E_FAIL;
		if(FAILED(device->CreateShaderResourceView(renderTexture, nullptr, &renderSRV)))
		   return E_FAIL;
		return S_OK;
	}
	//vec2 CalculatePercentageDifference(vec2 vec1, vec2 vec2)
	//{
	//	float xDiff = std::abs(vec1.x - vec2.x);
	//	float yDiff = std::abs(vec1.y - vec2.y);
	//	float xPercent = (vec1.x != 0) ? (xDiff / std::abs(vec1.x)) : 0;
	//	float yPercent = (vec1.y != 0) ? (yDiff / std::abs(vec1.y)) : 0;
	//	return { 1.0f - xPercent, 1.0f - yPercent };
	//}


	class ToolCamera* LinkCamera(void) { return camera; }

	ID3D11ShaderResourceView* mainView = nullptr;
private:
	bool focused = false, mouseHovering = false;
	ImVec2 windowSize;
	ImVec4 clear_color = {};
	ID3D11Device* device{nullptr};
	ID3D11DeviceContext* context{nullptr};
	Engine::DXDevice* dxDevice{nullptr};

	const D3D11_VIEWPORT* viewPort{ nullptr };
	class ToolCamera* camera{ nullptr };
	Engine::LineDrawer* lineDrawer{ nullptr };

	ID3D11RenderTargetView* mainViewRenderTarget{ nullptr };
	ID3D11Texture2D* backBufferTexture{nullptr};
	ID3D11Texture2D* renderTexture{nullptr};
	ID3D11RenderTargetView* renderRTV{nullptr};
	ID3D11ShaderResourceView* renderSRV{nullptr};
	float4 color = { 153.0f / 256.0f * 0.8f, 217.0f / 256.0f * 0.6f, 234.0f / 256.0f * 0.8f, 1.0f };


};