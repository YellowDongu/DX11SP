#include "pch.h"
#include "TitleBackground.h"

TitleBackground::TitleBackground(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

TitleBackground::TitleBackground(const TitleBackground& other) : Engine::UIObject(other)
{
}

void TitleBackground::Free(void)
{
	Engine::GameObject::Free();
	backGround.Free();
}

TitleBackground* TitleBackground::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	TitleBackground* newInstance = new TitleBackground(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* TitleBackground::Clone(void)
{
	TitleBackground* newInstance = new TitleBackground(*this);

	if (FAILED(newInstance->Awake()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"TitleBackground::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}									
HRESULT TitleBackground::Start(void)
{
	HRESULT result = CreateTransform();
	if (FAILED(result))
		return result;

	innerErrorCheck(backGround.LoadUITexture(L"../Bin/UI/Menu/Boot/Assets/Title_bg.png"), L"Load texture - Title_bg.png");
	CreateScale(backGround.texture, backGround.scale);
	innerErrorCheck(CreateVertex(backGround.vertexBuffer, backGround.indexBuffer, backGround.indexCount,Vector2::one()), L"Vertex/Index Create");



	return S_OK;
}

HRESULT TitleBackground::Awake(void)
{
	return S_OK;
}

void TitleBackground::FixedUpdate(void)
{
}

void TitleBackground::Update(void)
{
}

void TitleBackground::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void TitleBackground::Render(void)
{
	GetCurrentShader()->PassNumber(2);
	const D3D11_VIEWPORT& viewPortSetting = Device()->ViewPortInfomation();
	Vector2 scale = Vector2{ viewPortSetting.Width, viewPortSetting.Height };
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(Vector2::zero(), scale, 180.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, backGround.texture);
	GetCurrentShader()->Render(backGround.indexBuffer, backGround.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(backGround.indexCount, 0, 0);
	GetCurrentShader()->PassNumber(0);
}
