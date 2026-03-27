#include "pch.h"
#include "SpeedIndicator.h"
#include "FlightMovement.h"
#include "RMWR.h"

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
	newInstance->warning = static_cast<RMWR*>(player->GetComponent(L"RMWR"))->LinkMissileWarning();
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
	CreateScale(speedMeter.texture, scale);
	innerErrorCheck(CreateVertex(speedMeter.vertexBuffer, speedMeter.indexBuffer, speedMeter.indexCount, scale), L"Vertex/Index Create");

	::LoadFont(L"../Bin/ACES07.spritefont");
	::LoadFont(L"../Bin/ACES07KR.spritefont");
	text = ::CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;
	return S_OK;
}

HRESULT SpeedIndicator::Awake(void)
{
	return S_OK;
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

	Vector2 position = { windowSizeX * 0.5f * -0.35f, 0.0f };
	Vector2 localScale = Vector2::one();
	//localScale.y *= 1.5f;
	Matrix worldMatrix;

	if (*warning)
		color = { 1.0f, 0.0f, 0.0f, 1.0f };
	else
		color = { 0.0f, 1.0f, 0.0f, 1.0f };

	GetCurrentShader()->BindVariable("UIcolor", &color, sizeof(float4));

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, localScale, 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, speedMeter.texture);
	GetCurrentShader()->Render(speedMeter.indexBuffer, speedMeter.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(speedMeter.indexCount, 0, 0);

	position.x -= (scale.x * 0.45f);
	position.y += (scale.y * 0.25f);
	text->RenderText(std::to_wstring(static_cast<INT>(*velocity)), position, float2{ 1.0f, 1.0f }, color, 0.0f);

	position.x -= (scale.x * 0.05f);
	position.y += (scale.y) * 0.75f;
	text->RenderText(L"SPEED", position, float2{ 1.0f, 1.0f }, color, 0.0f);
}

