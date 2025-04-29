#include "pch.h"
#include "FCSHUD.h"
#include "FireControlSystem.h"

FireControlSystemHeadUpDisplay::FireControlSystemHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void FireControlSystemHeadUpDisplay::Free(void)
{
	standardLock.Free();
}

FireControlSystemHeadUpDisplay* FireControlSystemHeadUpDisplay::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player, Engine::GameObject* camera)
{
	FCSHUD* newInstance = new FCSHUD(dxDevice, dxDeviceContext);

	newInstance->lockStatus = &static_cast<FireControlSystem*>(player->GetComponent(L"FCS"))->LinkLockStatus();
	newInstance->target = &static_cast<FireControlSystem*>(player->GetComponent(L"FCS"))->LinkTarget();
	newInstance->camera = static_cast<Engine::Camera*>(camera->GetComponent(L"Camera"));
	newInstance->deviceInfomation = &::Device()->ViewPortInfomation();
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
	ErrMsg((std::wstring(L"FCSHUD::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \

HRESULT FireControlSystemHeadUpDisplay::Start(void)
{
	innerErrorCheck(standardLock.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_seeker01.dds"), L"Load texture - hud_seeker01.dds");
	CreateScale(standardLock.texture, markerScale);
	innerErrorCheck(CreateVertex(standardLock.vertexBuffer, standardLock.indexBuffer, standardLock.indexCount, markerScale), L"Vertex/Index Create");

	return S_OK;
}

HRESULT FireControlSystemHeadUpDisplay::Awake(void)
{
	return E_NOTIMPL;
}

void FireControlSystemHeadUpDisplay::FixedUpdate(void)
{
}

void FireControlSystemHeadUpDisplay::Update(void)
{
}

void FireControlSystemHeadUpDisplay::LateUpdate(void)
{
	if (*target != nullptr)
	{
		targetUIPosition = WorldToScreen((*target)->transform()->Position(), DirectX::XMLoadFloat4x4(&camera->ViewProjectionMatrix()), nullptr, inScreen);

		targetUIPosition.x *= (deviceInfomation->Width * 0.5f);
		targetUIPosition.y *= (deviceInfomation->Height * 0.5f);
	}

	AddRenderObject(RenderType::UI, this);
}

void FireControlSystemHeadUpDisplay::Render(void)
{
	if (*lockStatus == 1.0f)
		return;

	Vector2 scale = Vector2::one() * 0.7f;
	Vector2 screenPosition = targetUIPosition * ((1.0f - *lockStatus) * 0.5f + 0.5f);

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(screenPosition, scale, 0.0f));
	SetMatrix(world, worldMatrix);

	if (!*lockStatus)
	{
		GetCurrentShader()->BindVariable("UIcolor", &red, sizeof(float4));
	}

	SetTexture(diffuseTexture, standardLock.texture);
	GetCurrentShader()->Render(standardLock.indexBuffer, standardLock.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(standardLock.indexCount, 0, 0);

	if (!*lockStatus)
	{
		GetCurrentShader()->BindVariable("UIcolor", &defaultColor, sizeof(float4));
	}
}