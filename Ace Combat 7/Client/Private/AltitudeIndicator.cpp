#include "pch.h"
#include "AltitudeIndicator.h"

AltitudeIndicator::AltitudeIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void AltitudeIndicator::Free(void)
{
	AltMeter.Free();
	Base::Destroy(text);
}

AltitudeIndicator* AltitudeIndicator::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
	AltitudeIndicator* newInstance = new AltitudeIndicator(dxDevice, dxDeviceContext);
	newInstance->position = &player->transform()->Position();
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"AltitudeIndicator::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT AltitudeIndicator::Start(void)
{
	innerErrorCheck(AltMeter.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_alt_outline.png"), L"Load texture - hud_alt_outline.png");
	CreateScale(AltMeter.texture, scale);
	innerErrorCheck(CreateVertex(AltMeter.vertexBuffer, AltMeter.indexBuffer, AltMeter.indexCount, scale), L"FullModelVertex/Index Create");

	text = CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;
	return S_OK;
}
#undef innerErrorCheck

HRESULT AltitudeIndicator::Awake(void)
{
	return E_NOTIMPL;
}

void AltitudeIndicator::FixedUpdate(void)
{
}

void AltitudeIndicator::Update(void)
{
}

void AltitudeIndicator::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
	altitude = static_cast<INT>(position->y * 30.2f / 50.0f);
}

void AltitudeIndicator::Render(void)
{
	static const UINT SIUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";

	Vector2 position = { windowSizeX * 0.5f * 0.35f, 0.0f };
	Vector2 localScale = Vector2::one();
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, localScale, 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, AltMeter.texture);
	GetCurrentShader()->Render(AltMeter.indexBuffer, AltMeter.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(AltMeter.indexCount, 0, 0);

	position.x -= (scale.x * 0.35f);
	position.y += scale.y * 1.0f;
	text->RenderText(L"ALT", position, float2{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

	position.x += (scale.x * 0.5f);
	if (altitude >= 100)
		position.x -= 16.0f;
	if (altitude >= 1000)
		position.x -= 16.0f;
	// ¹Ù²ã¾ßµÊ => ±Û¾¾ ±æÀÌ Å½Áö¸¦ ÇÒ±î
	if (altitude >= 10000)
		position.x -= 16.0f;

	position.y -= (scale.y * 0.7f);
	text->RenderText(std::to_wstring(altitude), position, float2{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

}
