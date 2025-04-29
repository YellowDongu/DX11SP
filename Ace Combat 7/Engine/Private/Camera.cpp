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


void Camera::Free(void)
{
}


Camera* Camera::Create(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext, Transform*& transformComponent, float fov, float aspectRatio, float nearZ, float farZ)
{
	if (transformComponent == nullptr || dxDevice == nullptr || dxDeviceContext == nullptr)
		return nullptr;

	Camera* newInstance = new Camera(dxDevice, dxDeviceContext);
	newInstance->LinkTransform(transformComponent);
	if (FAILED(newInstance->SetCamera(fov, aspectRatio, nearZ, farZ)))
	{
		newInstance->Free();
		delete newInstance;
		return nullptr;
	}


	return newInstance;
}

Component* Camera::Clone(void)
{
	//Camera* newInstance = new Camera(*this);
	return nullptr;
}

HRESULT Camera::SetCamera(float _fov, float _aspectRatio, float _nearZ, float _farZ)
{
	fov = _fov;
	aspectRatio = _aspectRatio;
	nearZ = _nearZ;
	farZ = _farZ;
	return S_OK;
}

void Camera::Update(void)
{
}

void Camera::Render(void)
{
	if (transformComponent != nullptr)
	{
		Render(transformComponent->WorldMatrix());
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
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix));
	::GetCurrentShader()->PassNumber(0);
	::SetViewProjectionMatrix(viewProjectionMatrix);
	::EngineInstance()->ViewMatrix(viewMatrix);
	::EngineInstance()->ProjectionMatrix(projectionMatrix);
	return;
}

void Engine::Camera::Render(const Matrix& matrix)
{
	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&matrix);
	viewMatrix.r[0] = DirectX::XMVector3Normalize(viewMatrix.r[0]);
	viewMatrix.r[1] = DirectX::XMVector3Normalize(viewMatrix.r[1]);
	viewMatrix.r[2] = DirectX::XMVector3Normalize(viewMatrix.r[2]);
	viewMatrix = DirectX::XMMatrixInverse(nullptr, viewMatrix);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix));
	::GetCurrentShader()->PassNumber(0);
	::SetViewProjectionMatrix(viewProjectionMatrix);
	::EngineInstance()->ViewMatrix(viewMatrix);
	::EngineInstance()->ProjectionMatrix(projectionMatrix);
	return;
}

void Camera::LinkTransform(Transform*& transformComponent)
{
	position = &transformComponent->Position();
	angle = &transformComponent->Angle();
}

/*

Vector3 Transform::RotationAnglesToDirection(Vector3 angle)
{

		Vector3 result, angle = transformComponent->Angle(), worldForward = {0.0f, 0.0f, 1.0f};
		DirectX::XMStoreFloat3(&result, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&worldForward), DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(angle.x), DirectX::XMConvertToRadians(angle.y), DirectX::XMConvertToRadians(angle.z)))));


	Vector3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(worldForward.x, worldForward.y, worldForward.z, 0.0f), DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(angle.y), DirectX::XMConvertToRadians(angle.x), DirectX::XMConvertToRadians(angle.z))));
	return result;
}

void Transform::UpdateForward(const Vector3& angle)
{
	forward = RotationAnglesToDirection(angle);
	forwardCalculate = true;
}
void Transform::UpdateUp(const Vector3& forward)
{
	if (!forwardCalculate)
		UpdateForward(angle);

	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Cross(DirectX::XMVectorSet(forward.x, forward.y, forward.z, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)));
	upCalculate = true;
}
void Transform::UpdateRight(const Vector3& forward, const Vector3& up)
{
	if (!forwardCalculate)
		UpdateForward(angle);

	if (!upCalculate)
		UpdateUp(forward);

	DirectX::XMStoreFloat3(&right, DirectX::XMVector3Cross(DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f), DirectX::XMVectorSet(forward.x, forward.y, forward.z, 0.0f)));
	rightCalculate = true;
}*/