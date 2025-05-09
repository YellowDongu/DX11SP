#include "pch.h"
#include "GunReticle.h"
#include "FireControlSystem.h"
GunReticle::GunReticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void GunReticle::Free(void)
{
}

GunReticle* GunReticle::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	GunReticle* newInstance = new GunReticle(dxDevice, dxDeviceContext);
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
	ErrMsg((std::wstring(L"GunReticle::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT GunReticle::Start(void)
{
	innerErrorCheck(innerCircle.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_gunReticle.png"), L"Load texture - hud_gunReticle.png");
	CreateScale(innerCircle.texture, innerCircle.scale);
	innerErrorCheck(CreateVertex(innerCircle.vertexBuffer, innerCircle.indexBuffer, innerCircle.indexCount, innerCircle.scale), L"Vertex/Index Create");

	innerErrorCheck(outerCircle.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_gunReticle_inner.png"), L"Load texture - hud_gunReticle_inner.png");
	CreateScale(outerCircle.texture, outerCircle.scale);
	innerErrorCheck(CreateVertex(outerCircle.vertexBuffer, outerCircle.indexBuffer, outerCircle.indexCount, outerCircle.scale), L"Vertex/Index Create");

	return S_OK;
}

HRESULT GunReticle::Awake(void)
{
	Engine::GameObject* player = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"Ally")->GetGameObject(L"Player");
	FCS* fcs = static_cast<FCS*>(player->GetComponent(L"FCS"));
	target = &fcs->LinkTarget();
	transform = player->transform();
	maxDistance = ConvertFeetToWorld(1000.0f) * 5.0f;
	return S_OK;
}
#undef innerErrorCheck

void GunReticle::FixedUpdate(void)
{
}

void GunReticle::Update(void)
{
}

void GunReticle::LateUpdate(void)
{
	if (*target == nullptr)
		return;

	distance = ((*target)->transform()->Position() - transform->Position()).magnitude();
	insideDistance = distance <= maxDistance;
	if (insideDistance || Input()->getButton(KeyType::LCtrl))
		AddRenderObject(RenderType::UI, this);
}

void GunReticle::Render(void)
{
	Vector3 worldPosition;
	Matrix worldMatrix;
	static const UINT UIstride = sizeof(UIVertex);

	if (!insideDistance)
		worldPosition = transform->Position() + transform->Forward() * maxDistance;
	else
		worldPosition = transform->Position() + transform->Forward() * distance;

	bool inScreen;
	Vector2 screenPosition = Vector2(WorldToScreen(worldPosition, DirectX::XMLoadFloat4x4(&::EngineInstance()->RenderManager()->ViewProjectionMatrix()), &Device()->ViewPortInfomation(), inScreen, false));


	Vector2 calculatedPosition = screenPosition - innerCircle.scale * 0.5f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(calculatedPosition, scale, 0.0f));
	SetMatrix("worldMatrix", worldMatrix);

	SetTexture("diffuseTexture", innerCircle.texture);
	GetCurrentShader()->Render(innerCircle.indexBuffer, innerCircle.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(innerCircle.indexCount, 0, 0);


	calculatedPosition = screenPosition - outerCircle.scale * 0.5f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(calculatedPosition, scale, 0.0f));
	SetMatrix("worldMatrix", worldMatrix);

	SetTexture("diffuseTexture", outerCircle.texture);
	GetCurrentShader()->Render(outerCircle.indexBuffer, outerCircle.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(outerCircle.indexCount, 0, 0);
}
