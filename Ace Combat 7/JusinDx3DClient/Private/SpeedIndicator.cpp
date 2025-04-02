#include "pch.h"
#include "SpeedIndicator.h"
#include "FlightMovement.h"

SpeedIndicator::SpeedIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void SpeedIndicator::Free(void)
{
	speedMeter.Free();
	Base::Destroy(text);
}

SpeedIndicator* SpeedIndicator::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, float& velocity)
{
	SpeedIndicator* newInstance = new SpeedIndicator(dxDevice, dxDeviceContext);
	newInstance->velocity = &velocity;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

SpeedIndicator* SpeedIndicator::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
	FlightMovement* component = static_cast<FlightMovement*>(player->GetComponent(L"FlightMovement"));
	if (component == nullptr)
		return nullptr;

	SpeedIndicator* newInstance = new SpeedIndicator(dxDevice, dxDeviceContext);
	newInstance->velocity = &component->Velocity();
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
	ErrMsg((std::wstring(L"SpeedIndicator::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT SpeedIndicator::Start(void)
{
	innerErrorCheck(speedMeter.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_spd_outline.png"), L"Load texture - hud_spd_outline.png");
	CreateScale(speedMeter.texture, scale); scale *= 0.5f;
	innerErrorCheck(CreateVertex(speedMeter.vertexBuffer, speedMeter.indexBuffer, speedMeter.indexCount, scale), L"Vertex/Index Create");

	LoadFont(L"../Bin/ACES07.spritefont");
	LoadFont(L"../Bin/ACES07KR.spritefont");
	text = CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;
	return S_OK;
}

HRESULT SpeedIndicator::Awake(void)
{
	return E_NOTIMPL;
}
#undef innerErrorCheck

void SpeedIndicator::FixedUpdate(void)
{
}

void SpeedIndicator::Update(void)
{
}

void SpeedIndicator::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void SpeedIndicator::Render(void)
{
	static const UINT SIUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";

	Vector2 position = {-100.0f, 0.0f};
	//Vector2 scale = Vector2::one() * 0.5f;
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, speedMeter.texture);
	GetCurrentShader()->Render(speedMeter.indexBuffer, speedMeter.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(speedMeter.indexCount, 0, 0);

	position.x -= (scale.x * 0.675f) * 2.0f;
	position.y += (scale.y * 0.75f);
	text->RenderText(std::to_wstring(static_cast<INT>(*velocity)), position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

	position.x -= (scale.x * 0.025f) * 2.0f;
	position.y += (scale.y) * 2.1f;
	text->RenderText(L"SPEED", position, float2{ 0.65f, 1.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
}

