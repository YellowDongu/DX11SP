#include "Foundation.h"
#include "PointDrawer.h"
#include "GameInstance.h"

using namespace Engine;

PointDrawer::PointDrawer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext)
{
}

PointDrawer::PointDrawer(const PointDrawer& other) : Component(other), sphere(DirectX::GeometricPrimitive::CreateSphere(dxDeviceContext, 1.0f))
{
}

void PointDrawer::Free(void)
{
}

PointDrawer* PointDrawer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, FLOAT radius)
{
	PointDrawer* newInstance = new PointDrawer(dxDevice, dxDeviceContext);
	newInstance->radius = radius;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* PointDrawer::Clone(void)
{
	return new PointDrawer(*this);
}

HRESULT PointDrawer::Start(void)
{
	//std::unique_ptr<DirectX::GeometricPrimitive> newSphere = DirectX::GeometricPrimitive::CreateSphere(dxDeviceContext, 1.0f);
	//sphere = std::move(newSphere);
	sphere = std::move(DirectX::GeometricPrimitive::CreateSphere(dxDeviceContext, radius));
	viewPort = &Device()->ViewPortInfomation();
	return S_OK;
}

void PointDrawer::Update(void)
{
}

void PointDrawer::LateUpdate(void)
{
}

void PointDrawer::FixedUpdate(void)
{
}

void PointDrawer::Render(void)
{
	sphere->Draw(DirectX::XMMatrixTranslation(position.x, position.y, position.z), DirectX::XMLoadFloat4x4(&::EngineInstance()->ViewMatrix()), DirectX::XMLoadFloat4x4(&::EngineInstance()->ProjectionMatrix()), DirectX::XMLoadFloat4(&color));
}

void Engine::PointDrawer::ChangeRadius(FLOAT value)
{
	sphere.reset();
	radius = value;
	sphere = std::move(DirectX::GeometricPrimitive::CreateSphere(dxDeviceContext, value));
}
