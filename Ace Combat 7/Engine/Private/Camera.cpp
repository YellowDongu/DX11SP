#include "Foundation.h"
#include "Camera.h"
#include "Transform.h"
#include "GameInstance.h"

using namespace Engine;

Vector3 DefaultForward = { 0.0f, 0.0f, 1.0f };

Camera::Camera(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext) : Component(dxDevice, dxDeviceContext),
fov(45.0f), aspectRatio(1.3f), nearZ(0.1f), farZ(1000.0f),
position(nullptr), angle(nullptr),
lookAt{ 0.0f,0.0f,0.0f }, up{ 0.0f,0.0f,0.0f },
viewProjectionMatrix{}
{
	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixIdentity());
}

Engine::Camera::Camera(const Camera& other) : Component(other), fov(other.fov), aspectRatio(other.aspectRatio), nearZ(other.nearZ), farZ(other.farZ), lookAt(other.lookAt), up(other.up), viewProjectionMatrix(other.viewProjectionMatrix), projectionMatrix(other.projectionMatrix), angle(nullptr), position(nullptr)
{
}


void Camera::Free(void)
{
}


Camera* Camera::Create(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext, float fov, float aspectRatio, float nearZ, float farZ)
{
	Camera* newInstance = new Camera(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->SetCamera(fov, aspectRatio, nearZ, farZ)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Component* Camera::Clone(void)
{
	return new Camera(*this);
}

HRESULT Camera::SetCamera(float _fov, float _aspectRatio, float _nearZ, float _farZ)
{
	fov = _fov;
	aspectRatio = _aspectRatio;
	nearZ = _nearZ;
	farZ = _farZ;
	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ));
	::SetViewProjectionMatrix(::EngineInstance()->RenderManager()->ViewMatrix(), projectionMatrix);
	return S_OK;
}

void Camera::LateUpdate(void)
{
	::BindCamera(this);
}

void Camera::Render(void)
{
	if (gameObject != nullptr && gameObject->transform() != nullptr)
	{
		gameObject->transform()->UpdateWorldMatrix();
		Render(gameObject->transform()->WorldMatrix());
		return;
	}


	if (position == nullptr || angle == nullptr)
		return;

	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(angle->x), DirectX::XMConvertToRadians(angle->y), DirectX::XMConvertToRadians(angle->z));

	static DirectX::XMVECTOR defaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static DirectX::XMVECTOR defaultUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);     

	DirectX::XMStoreFloat3(&focusPosition, DirectX::XMVector3Rotate(defaultForward, quaternion));

	Vector3 focusPositionSave = focusPosition;

	focusPosition += *position;
	while (focusPosition == *position)
	{
		float x = focusPosition.x + focusPositionSave.x;
		focusPosition.x = x;

		focusPosition.y = focusPosition.y + focusPositionSave.y;
		focusPosition.z = focusPosition.z + focusPositionSave.z;
	}
	

	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(position), DirectX::XMLoadFloat3(&focusPosition), DirectX::XMVector3Rotate(defaultUp, quaternion));
	DirectX::XMMATRIX projectionXmMatrix = DirectX::XMLoadFloat4x4(&projectionMatrix);

	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, projectionXmMatrix));
	::SetViewProjectionMatrix(viewMatrix, projectionXmMatrix);
}

void Engine::Camera::Render(const Matrix& matrix)
{
	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&matrix);
	viewMatrix.r[0] = DirectX::XMVector3Normalize(viewMatrix.r[0]);
	viewMatrix.r[1] = DirectX::XMVector3Normalize(viewMatrix.r[1]);
	viewMatrix.r[2] = DirectX::XMVector3Normalize(viewMatrix.r[2]);
	viewMatrix = DirectX::XMMatrixInverse(nullptr, viewMatrix);
	DirectX::XMMATRIX projectionXmMatrix = DirectX::XMLoadFloat4x4(&projectionMatrix);

	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, projectionXmMatrix));
	::SetViewProjectionMatrix(viewMatrix, projectionXmMatrix);
}