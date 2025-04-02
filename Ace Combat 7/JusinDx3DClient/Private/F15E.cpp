#include "pch.h"
#include "F15E.h"

F15E::F15E(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : GameObject(dxDevice, dxDeviceContext)
{
}

F15E::F15E(const F15E& other) : GameObject(other)
{
}

void F15E::Free(void)
{
}

F15E* F15E::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	F15E* newInstance = new F15E(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* F15E::Clone(void)
{
	return new F15E(*this);
}

HRESULT F15E::Start(void)
{
    CreateTransform();
    // 모델을 어떤걸 선택했는지 정보를 받고 모댈 정보 프리팹 가져와서 여기다가 저장
    model = Engine::Model::Create(dxDevice, dxDeviceContext, "..\\Bin\\f15e.fbx");
    if (model == nullptr)
        return E_FAIL;

    Matrix matrix;
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)));
    model->SetGlobalOffset(matrix);
    AddComponent(model, L"FullModel");

    transformComponent->Scale() = Vector3::one() * 0.1f;
    transformComponent->Position().x = 50.0f;
    transformComponent->Position().y = 48.0f;
    transformComponent->Position().z = 55.0f;

	return S_OK;
}

void F15E::Update(void)
{
}

void F15E::LateUpdate(void)
{
}

void F15E::FixedUpdate(void)
{
}

void F15E::Render(void)
{
}
