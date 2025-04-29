#pragma once

#include "Base.h"

namespace Engine
{
	class ENGINEDLL DXDevice final : public Base
	{
	private:
		DXDevice(void);
		DXDevice(DXDevice& other) = delete;
		virtual ~DXDevice(void) = default;
		virtual void Free(void);
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
	private:

		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* backBuffer;
		ID3D11DepthStencilView* depthStencilView;

		float4 backBufferColor;
		DeviceInfomation deviceInfo;
		D3D11_VIEWPORT viewPortDesc;
	};
}



