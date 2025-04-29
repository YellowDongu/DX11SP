#include "pch.h"
#include "MainCamera.h"

MainCamera::MainCamera(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : GameObject(dxDevice, dxDeviceContext), camera(nullptr)
{
}

MainCamera::MainCamera(const MainCamera& other) : GameObject(other), camera(nullptr)
{
}

void MainCamera::Free(void)
{
	GameObject::Free();
}

HRESULT MainCamera::Start(void)
{	
	if (FAILED(CreateTransform()))
		return E_FAIL;

	transformComponent->Position() = Vector3{ 50.0f, 50.0f, 0.0f };

	camera = Engine::Camera::Create(dxDevice, dxDeviceContext, transformComponent, 45.0f, static_cast<FLOAT>(windowSizeX) / static_cast<FLOAT>(windowSizeY), 0.1f, 50000.0f);
	if (!camera)
		return E_FAIL;
	AddComponent(camera, L"Camera");

	return S_OK;
}

HRESULT MainCamera::Awake(void)
{
	camera = Engine::Camera::Create(dxDevice, dxDeviceContext, transformComponent);
	if (camera == nullptr)
		return E_FAIL;

	AddComponent(camera, L"Camera");


	return S_OK;
}

MainCamera* MainCamera::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MainCamera* newInstance = new MainCamera(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

MainCamera* MainCamera::Clone(void)
{
	MainCamera* newInstance = new MainCamera(*this);
	if (FAILED(newInstance->Awake()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

void MainCamera::Update(void)
{


}

void MainCamera::LateUpdate(void)
{
	//camera->Render();
	AddRenderObject(RenderType::Camera, this);
}

void MainCamera::FixedUpdate(void)
{
}

void MainCamera::Render(void)
{
	//transformComponent->UpdateWorldMatrix();
	camera->Render(transformComponent->WorldMatrix());
}
