
#include "Foundation.h"
#include "DXDevice.h"

using namespace Engine;

DXDevice::DXDevice(void) : dxDevice(nullptr), dxDeviceContext(nullptr), swapChain(nullptr), backBuffer(nullptr), depthStencilView(nullptr), backBufferColor({ 0.0f,0.0f,0.0f,0.0f})
{
}


DXDevice* DXDevice::Create(DeviceInfomation& deviceInfo)
{
	DXDevice* newInstance = new DXDevice();
	if (FAILED(newInstance->StartDevice(deviceInfo)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	newInstance->deviceInfo = deviceInfo;
	newInstance->backBufferColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	return newInstance;
}


HRESULT DXDevice::StartDevice(DeviceInfomation& deviceInfo)
{
	UINT flag = 0;

#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL FeatureLV;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, nullptr, 0, D3D11_SDK_VERSION, &dxDevice, &FeatureLV, &dxDeviceContext)))
		return E_FAIL;

	if (FAILED(StartSwapChain(dxDevice, swapChain, deviceInfo)))
		return E_FAIL;

	if (FAILED(StartBackBufferRenderTargetView(dxDevice, backBuffer, swapChain)))
		return E_FAIL;

	if (FAILED(StartDepthStencilView(dxDevice, depthStencilView, deviceInfo.viewpoirtWidth, deviceInfo.viewpoirtLength)))
		return E_FAIL;

	ID3D11RenderTargetView* pRTVs[1] = {
		backBuffer,
	};

	dxDeviceContext->OMSetRenderTargets(1, pRTVs, depthStencilView);
	deviceInfo.hWnd;
	deviceInfo.isWindowed;
	deviceInfo.viewpoirtWidth;
	deviceInfo.viewpoirtLength;

	ZeroMemory(&viewPortDesc, sizeof(D3D11_VIEWPORT));
	viewPortDesc.TopLeftX = 0;
	viewPortDesc.TopLeftY = 0;
	viewPortDesc.Width = (FLOAT)deviceInfo.viewpoirtWidth;
	viewPortDesc.Height = (FLOAT)deviceInfo.viewpoirtLength;
	viewPortDesc.MinDepth = 0.f;
	viewPortDesc.MaxDepth = 1.f;

	dxDeviceContext->RSSetViewports(1, &viewPortDesc);
	return S_OK;
}
HRESULT DXDevice::StartSwapChain(ID3D11Device*& dxDevice, IDXGISwapChain*& swapChain, DeviceInfomation deviceInfo)
{
	IDXGIDevice* giDevice = nullptr;
	IDXGIAdapter* giAdapter = nullptr;
	IDXGIFactory* giFactory = nullptr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&giDevice);
	giDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&giAdapter);
	giAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&giFactory);

	swapChainDesc.BufferDesc.Width = deviceInfo.viewpoirtWidth;
	swapChainDesc.BufferDesc.Height = deviceInfo.viewpoirtLength;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.OutputWindow = deviceInfo.hWnd;
	swapChainDesc.Windowed = deviceInfo.isWindowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(giFactory->CreateSwapChain(dxDevice, &swapChainDesc, &swapChain)))
		return E_FAIL;

	giFactory->Release();
	giAdapter->Release();
	giDevice->Release();

	return S_OK;
}


HRESULT DXDevice::StartBackBufferRenderTargetView(ID3D11Device*& dxDevice, ID3D11RenderTargetView*& backBuffer, IDXGISwapChain*& swapChain)
{
	if (dxDevice == nullptr)
		return E_FAIL;

	ID3D11Texture2D* backBufferTexture = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
		return E_FAIL;
	if (FAILED(dxDevice->CreateRenderTargetView(backBufferTexture, nullptr, &backBuffer)))
		return E_FAIL;

	backBufferTexture->Release();

	return S_OK;
}

HRESULT DXDevice::StartDepthStencilView(ID3D11Device*& dxDevice, ID3D11DepthStencilView*& depthStencilView, UINT windowWidth, UINT windowHeight)
{
	if (dxDevice == nullptr)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = windowWidth;
	TextureDesc.Height = windowHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(dxDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(dxDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &depthStencilView)))
		return E_FAIL;

	pDepthStencilTexture->Release();
	pDepthStencilTexture = nullptr;

	return S_OK;
}

HRESULT DXDevice::CreateImGuiView(ID3D11ShaderResourceView*& mainView)
{
	if (swapChain == nullptr)
		return E_FAIL;

	ID3D11Texture2D* backBufferTexture;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
		return E_FAIL;

	if (FAILED(dxDevice->CreateShaderResourceView(backBufferTexture, nullptr, &mainView)))
		return E_FAIL;

	backBufferTexture->Release();

	return S_OK;
}


void DXDevice::Render(std::function<void(void)> renderFunction)
{
	if (dxDeviceContext == nullptr)
	{
		if (FAILED(StartDevice(deviceInfo)))
		{
			Free();
			PostQuitMessage(0);
		}
	}
	if (swapChain == nullptr)
	{
		if (FAILED(StartSwapChain(dxDevice, swapChain, deviceInfo)))
		{
			Free();
			PostQuitMessage(0);
		}

	}

	dxDeviceContext->ClearRenderTargetView(backBuffer, (FLOAT*)&backBufferColor);
	dxDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


	renderFunction();


	HRESULT hr = swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			RestartDevice();
		}
	}
}

void DXDevice::RestartDevice(void)
{
	HRESULT deviceLostReason = dxDevice->GetDeviceRemovedReason();
	if (deviceLostReason == DXGI_ERROR_DEVICE_HUNG || deviceLostReason == DXGI_ERROR_DEVICE_REMOVED || deviceLostReason == DXGI_ERROR_DEVICE_RESET)
	{
		Free();
		StartDevice(deviceInfo);
	}
	else
	{
		if (swapChain)
			swapChain->Release();
		swapChain = nullptr;
		if (depthStencilView)
			depthStencilView->Release();
		depthStencilView = nullptr;
		if (backBuffer)
			backBuffer->Release();
		backBuffer = nullptr;


		if (FAILED(StartSwapChain(dxDevice, swapChain, deviceInfo)))
		{
			Free();
			PostQuitMessage(0);
			return;
		}

		if (FAILED(StartBackBufferRenderTargetView(dxDevice, backBuffer, swapChain)))
		{
			Free();
			PostQuitMessage(0);
			return;
		}

		if (FAILED(StartDepthStencilView(dxDevice, depthStencilView, deviceInfo.viewpoirtWidth, deviceInfo.viewpoirtLength)))
		{
			Free();
			PostQuitMessage(0);
			return;
		}

		ID3D11RenderTargetView* pRTVs[1] = {
			backBuffer,
		};

		dxDeviceContext->OMSetRenderTargets(1, pRTVs, depthStencilView);
		deviceInfo.hWnd;
		deviceInfo.isWindowed;
		deviceInfo.viewpoirtWidth;
		deviceInfo.viewpoirtLength;

		ZeroMemory(&viewPortDesc, sizeof(D3D11_VIEWPORT));
		viewPortDesc.TopLeftX = 0;
		viewPortDesc.TopLeftY = 0;
		viewPortDesc.Width = (FLOAT)deviceInfo.viewpoirtWidth;
		viewPortDesc.Height = (FLOAT)deviceInfo.viewpoirtLength;
		viewPortDesc.MinDepth = 0.f;
		viewPortDesc.MaxDepth = 1.f;

		dxDeviceContext->RSSetViewports(1, &viewPortDesc);
	}
}

void DXDevice::Free(void)
{
	dxDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (swapChain)
		swapChain->Release();
	if (depthStencilView)
	depthStencilView->Release();
	if (backBuffer)
	backBuffer->Release();
	if (dxDeviceContext)
	{
		dxDeviceContext->ClearState();
		dxDeviceContext->Flush();
		dxDeviceContext->Release();
	}

	
#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug* d3dDebug = nullptr;
	HRESULT hr = dxDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	}
	if (d3dDebug != nullptr)
		d3dDebug->Release();

	ULONG counter = 0;

	if (dxDevice)
		counter = dxDevice->Release();

	if (counter != 0)
		int alarm = 0;

#elif
	if (dxDevice)
		dxDevice->Release();
#endif
}

