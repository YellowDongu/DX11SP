#pragma once

#include "Base.h"

namespace Engine
{
	class RenderTarget : public Base
	{
	public:
		struct RenderTargetInfomation
		{
			float4 clearColor = {};
			UINT xSize;
			UINT ySize;
			DXGI_FORMAT pixelFormat;
		};
	private:
		RenderTarget(void) = delete;
		RenderTarget(ID3D11Device* device, ID3D11DeviceContext* context);
		RenderTarget(const RenderTarget& other) = delete;
		virtual ~RenderTarget(void) = default;
		virtual void Free(void) override;
	public:
		static RenderTarget* Create(ID3D11Device* device, ID3D11DeviceContext* context, const RenderTargetInfomation& infomation);
		void Clear(void) { context->ClearRenderTargetView(renderTargetView, (FLOAT*)&infomation.clearColor); }
		ID3D11ShaderResourceView* GetShaderResourceView(void) { return shaderResourceView; }
		ID3D11RenderTargetView* GetRenderTargetView(void) { return renderTargetView; }

	private:
		HRESULT Start(void);
		HRESULT CreateRenderTarget(UINT iSizeX, UINT iSizeY, DXGI_FORMAT ePixelFormat);

		ID3D11Device* device = { nullptr };
		ID3D11DeviceContext* context = { nullptr };

		ID3D11Texture2D* texture = { nullptr };
		ID3D11RenderTargetView* renderTargetView = { nullptr };
		ID3D11ShaderResourceView* shaderResourceView = { nullptr };

		RenderTargetInfomation infomation;
		Matrix debugWorldMatrix{};
	};

	enum class RenderTargetType
	{
		Diffuse,
		Normal,
		Shade,
		Depth,
		Specular,
		End
	};
	enum class DeferredRenderOrder
	{
		Object,
		Shade,
		Blend,
		End
	};

	class ENGINEDLL DXDevice final : public Base
	{
	private:
		DXDevice(void);
		DXDevice(DXDevice& other) = delete;
		virtual ~DXDevice(void) = default;
		virtual void Free(void) override;
	public:
		static DXDevice* Create(DeviceInfomation& deviceInfo);

		HRESULT StartDevice(DeviceInfomation& deviceInfo);
		HRESULT StartSwapChain(ID3D11Device*& dxDevice, IDXGISwapChain*& swapChain, DeviceInfomation deviceInfo, bool shaderOutput = false);
		HRESULT StartBackBufferRenderTargetView(ID3D11Device*& dxDevice, ID3D11RenderTargetView*& backBuffer, IDXGISwapChain*& swapChain);
		HRESULT StartDepthStencilView(ID3D11Device*& dxDevice, ID3D11DepthStencilView*& depthStencilView, UINT windowWidth, UINT windowHeight);
		HRESULT CreateImGuiView(ID3D11ShaderResourceView*& mainView);

		void Render(std::function<void(void)> renderFunction);
		void RestartDevice(void);

		void SetBackBufferColor(float4 color) { backBufferColor = color; }

		void GetDevices(ID3D11Device*& devicePointer, ID3D11DeviceContext*& contextPointer) { devicePointer = dxDevice; contextPointer = dxDeviceContext; }
		ID3D11Device* GetDevice(void) { return dxDevice; }
		ID3D11DeviceContext* GetDeviceContext(void) { return dxDeviceContext; }
		const D3D11_VIEWPORT& ViewPortInfomation(void) const { return viewPortDesc; }
		ID3D11DepthStencilView* GetDepthStencilView(void) { return depthStencilView; }
		IDXGISwapChain* GetDXSwapChain(void) { return swapChain; }
		ID3D11RenderTargetView* GetTargetView(void) { return backBuffer; }

		HRESULT AddRenderTargets(RenderTargetType type, const RenderTarget::RenderTargetInfomation& infomation, bool replaceMode = false);
		HRESULT BindMultiRenderTarget(DeferredRenderOrder renderOrder, RenderTargetType type);
		HRESULT SetMultiRenderTarget(DeferredRenderOrder renderOrder);
		HRESULT ApplyBackBuffer(void);
		RenderTarget* GetRenderTarget(RenderTargetType type) { return renderTargets[static_cast<UINT>(type)]; }

	private:
		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* backBuffer;
		ID3D11DepthStencilView* depthStencilView;

		float4 backBufferColor;
		DeviceInfomation deviceInfo;
		D3D11_VIEWPORT viewPortDesc;

		std::map<DeferredRenderOrder, std::list<RenderTargetType>> multiRenderTarget;
		std::vector<RenderTarget*> renderTargets;
	};
}



